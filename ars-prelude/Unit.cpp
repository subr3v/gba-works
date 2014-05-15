#include "Unit.h"

#include "gba.h"
#include "GameConstants.h"
#include "GameController.h"
#include "ObjectGraphics.h"
#include <stdio.h>
#include "UnitDescription.h"
#include "GameWorld.h"

#include "MovementCommand.h"
#include "AttackCommand.h"
#include "WaitCommand.h"
#include "UseItemCommand.h"

#include "GameObjectFactory.h"

Unit::Unit(UnitType unitType, GameController* gameController) : GameObject(gameController, Object_Unit)
{
    currentFrame = 0;
    numberOfUpgrades = 0;
    experience = 0;
    level = 1;
    gold = 0;
    food = MAX_FOOD;
    poisonTimer = sleepTimer = immobilizeTimer = 0;
    EnableHumanControlled(false); // By default it's an AI monster.
    itemToUse = ItemType_Count;
    const UnitDescription& description = GameObjectFactory::GetUnitDescription(unitType);
    this->unitType = unitType;
    this->unitDescription = description;
    healthPoints = maxHealthPoints = description.startingHealth;
    magicPoints = maxMagicPoints = description.startingMagic;
    physicalAttack = description.physicalAttack;
    physicalDefense = description.physicalDefense;
    magicalAttack = description.magicalAttack;
    magicalDefense = description.magicalDefense;
    attackEffect = description.attackEffect;
    SetSpriteIdentifier(description.spriteIdentifier);
}

Unit::~Unit()
{

}

bool Unit::IsCellEmpty( uint16_t x, uint16_t y )
{
    uint16_t xCell = x / TILE_SIZE;
    uint16_t yCell = y / TILE_SIZE;
    return gameController->IsCellEmpty(xCell, yCell, true);
}

void Unit::DamageUnit( Unit* damagedUnit )
{
    int damage = physicalAttack * (1.9 + (0.7 * ( 1 - ( physicalAttack / 999) ) ) ) - (damagedUnit->physicalDefense / 3) + 5;

    if ( attackEffect == Attack_Effect_Random_Damage )
    {
        damage += Random(-damage / 2, damage / 2);
    }

    if ( damage < 0 ) damage = 0;
    if ( damage > 0 )
    {
        if ( attackEffect == Attack_Effect_Drain_Hp )
        {
            this->ModifyHealth(damage / 4);
        }
        if ( attackEffect == Attack_Effect_Drain_Mp )
        {
            this->ModifyMagic(damage / 4);
            damagedUnit->ModifyMagic(-damage / 4);
        }

        damagedUnit->ModifyHealth(-damage);
        gameController->DisplayUnitDamage(damagedUnit, damage);
    }

    if (damagedUnit->IsDead())
    {
        this->GiveExperience(damagedUnit->GetExperienceGiven());
    }
    else
    {
        // Status ailment effects: always 20% success chance
        bool specialEffectSuccess = Random(0, 100) <= 20;
        if ( specialEffectSuccess )
        {
            switch (attackEffect)
            {
                case Attack_Effect_Teleport:
                    damagedUnit->position = gameController->GetRandomPositionWorld(false);
                    break;
                case Attack_Effect_Sleep:
                    damagedUnit->InflictSleep();
                    break;
                case Attack_Effect_Immobilize:
                    damagedUnit->InflictImmobilize();
                    break;
                case Attack_Effect_Poison:
                    damagedUnit->InflictPoison();
                    break;
                default:
                    break;
            }
        }
    }

    if ( damagedUnit == gameController->GetPlayer() )
        gameController->ShakeCamera();
}

void Unit::ModifyHealth( int32_t health )
{
    if ( health < 0 )
        sleepTimer = 0; // We awake after receiving damage no matter what.

    this->healthPoints += health;
    if ( healthPoints >= maxHealthPoints )
        healthPoints = maxHealthPoints;

    if ( healthPoints <= 0 )
    {
        healthPoints = 0;
        if ( isControlledByHuman == false )
        {
            Kill();
            //Drop item or gold.

            ItemType objectType = unitDescription.droppedItem; // 65 % gold , 35% item
            if ( Random(0, 100) <= 65 && objectType != ItemType_Stairs) // You can't override stairs drop.
            {
                if ( level < 5 )
                    objectType = ItemType_Gold_Small;
                else if ( level < 10 )
                    objectType = ItemType_Gold_Medium;
                else if ( level < 20 )
                    objectType = ItemType_Gold_Big;
                else
                    objectType = ItemType_Gold_VeryBig;
            }
            GameObject* droppedObject = (GameObject*)GameObjectFactory::CreateItem(gameController, objectType);
            droppedObject->position = this->position;
            gameController->AddObject(droppedObject);
        }
    }

    if ( healthPoints < maxHealthPoints / 3 )
    {
        DisplayAnimation(Injured);
    }
}

void Unit::ModifyMagic( int32_t magic )
{
    this->magicPoints += magic;
    if ( magicPoints >= maxMagicPoints )
        magicPoints = maxMagicPoints;
    if ( magicPoints < 0 )
        magicPoints = 0;
}

void Unit::LevelUp()
{
    if ( level >= 100 )
        return;

    maxHealthPoints += unitDescription.healthGrowth;
    maxMagicPoints += unitDescription.magicGrowth;
    healthPoints += unitDescription.healthGrowth;
    magicPoints += unitDescription.magicGrowth;

    physicalAttack += unitDescription.physicalAttackGrowth;
    physicalDefense += unitDescription.physicalDefenseGrowth;
    magicalAttack += unitDescription.magicalAttackGrowth;
    magicalDefense += unitDescription.magicalDefenseGrowth;

    level++;
    if ( IsHumanControlled() )
    {
        gameController->ShowText("%s is level %d now!", unitDescription.Name, level);
        gameController->DisplayUnitLevelUp(this);
    }
}

void Unit::LevelUpTo( int16_t targetLevel )
{
    while (level < targetLevel)
        LevelUp();
}

void Unit::GiveExperience( int16_t experience )
{
    //gameController->ShowText("%s receives %d experience",unitDescription.Name,experience);
    this->experience += experience;
    if ( this->experience >= GetNeededExperience() && level < 100)
    {
        this->experience = 0;
        LevelUp();
    }
}

int Unit::GetExperienceGiven()
{
    return unitDescription.experienceGiven + level * 2;
}

Command* Unit::ThinkCommand()
{
    Command* command = NULL;

    if ( sleepTimer > 0 )
    {
        sleepTimer--;
        command = new WaitCommand(this);
    }
    else if ( GetCurrentAnimation() == Sleep )
    {
        DisplayAnimation(Idle);
    }

    if ( command == NULL && itemToUse != ItemType_Count )
    {
        command = new UseItemCommand(this, itemToUse);
        itemToUse = ItemType_Count;
    }

    if ( command == NULL )
    {
        if ( isControlledByHuman )
            command = ThinkHuman();
        else
            command = ThinkAI();
    }

    if ( command != NULL )
    {
        stepTimer++;
        if (stepTimer == 2)
        {
            food--;
            if (food < 0)
            {
                food = 0;
                ModifyHealth(-1); // You lose one health per action.
            }
            stepTimer = 0;
        }

        if ( immobilizeTimer > 0 )
        {
            immobilizeTimer--;
        }

        if ( poisonTimer > 0 )
        {
            int32_t poisonDamage = healthPoints / 20;
            // Poison can't kill.
            if ( healthPoints - poisonDamage > 0 )
                ModifyHealth(-poisonDamage);
            poisonTimer--;
        }
    }
    return command;
}

Command* Unit::ThinkHuman()
{
    Command* command = NULL;
    if ( hardware::IsButtonDown(KEY_B) )
    {
        if ( hardware::IsButtonDown(KEY_RIGHT))
            this->currentDirection = East;
        if ( hardware::IsButtonDown(KEY_LEFT))
            this->currentDirection = West;
        if ( hardware::IsButtonDown(KEY_DOWN))
            this->currentDirection = South;
        if ( hardware::IsButtonDown(KEY_UP))
            this->currentDirection = North;
        if ( hardware::IsButtonPressed(KEY_A))
            command = new WaitCommand(this);
    }
    else
    {
        if ( hardware::IsButtonDown(KEY_RIGHT) )
        {
            currentDirection = East;
            if ( IsCellEmpty(position.x + TILE_SIZE, position.y) )
                command = new MovementCommand(this, East);
        }
        else if ( hardware::IsButtonDown(KEY_LEFT)  )
        {
            currentDirection = West;
            if ( IsCellEmpty(position.x - TILE_SIZE, position.y) )
                command = new MovementCommand(this, West);
        }
        else if ( hardware::IsButtonDown(KEY_UP) )
        {
            currentDirection = North;
            if ( IsCellEmpty(position.x , position.y - TILE_SIZE) )
                command = new MovementCommand(this, North);
        }
        else if ( hardware::IsButtonDown(KEY_DOWN) )
        {
            currentDirection = South;
            if ( IsCellEmpty(position.x, position.y + TILE_SIZE) )
                command = new MovementCommand(this, South);
        }
        if ( hardware::IsButtonPressed(KEY_A) )
        {
            int16_t xAttack = position.x + GetHorizontalComponent(currentDirection) * TILE_SIZE;
            int16_t yAttack = position.y + GetVerticalComponent(currentDirection) * TILE_SIZE;
            if ( gameController->IsCellWalkable(xAttack / TILE_SIZE, yAttack / TILE_SIZE))
                command = new AttackCommand(this, this->currentDirection);
        }
        /*
        if( hardware::IsButtonPressed(KEY_R) )
        {
        	int16_t xAttack = position.x + GetHorizontalComponent(currentDirection) * TILE_SIZE;
        	int16_t yAttack = position.y + GetVerticalComponent(currentDirection) * TILE_SIZE;
        	if( gameController->IsCellWalkable(xAttack / TILE_SIZE,yAttack / TILE_SIZE))
        		command = new TeleportCommand(this);
        }
        if( hardware::IsButtonPressed(KEY_L) )
        {
        	int16_t xAttack = position.x + GetHorizontalComponent(currentDirection) * TILE_SIZE;
        	int16_t yAttack = position.y + GetVerticalComponent(currentDirection) * TILE_SIZE;
        	if( gameController->IsCellWalkable(xAttack / TILE_SIZE,yAttack / TILE_SIZE))
        		command = new ControlCreatureCommand(this,currentDirection);
        }*/
    }

    if ( hardware::IsButtonPressed(KEY_START))
    {
        gameController->OpenShop();
    }

    return command;
}

Command* Unit::ThinkAI()
{
    Point16 cellPosition = GetGridPosition();


    GameWorld* world = gameController->GetWorld();
    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        GameObject* obj = world->objects[i];
        if ( obj == NULL || obj->AsUnit() == NULL || obj->AsUnit()->IsHumanControlled() == false )
            continue;

        Point16 targetPosition = obj->AsUnit()->GetGridPosition();

        if ( cellPosition.x == targetPosition.x )
        {
            if ( cellPosition.y + 1 == targetPosition.y )
                return new AttackCommand(this, South);
            else if ( cellPosition.y - 1 == targetPosition.y )
                return new AttackCommand(this, North);
        }
        else if ( cellPosition.y == targetPosition.y )
        {
            if ( cellPosition.x + 1 == targetPosition.x )
                return new AttackCommand(this, East);
            else if ( cellPosition.x - 1 == targetPosition.x )
                return new AttackCommand(this, West);
        }

        if ( cellPosition.x < targetPosition.x )
        {
            if (gameController->IsCellEmpty(cellPosition.x + 1, cellPosition.y, true))
                return new MovementCommand(this, East);
        }
        else if ( cellPosition.x > targetPosition.x )
        {
            if (gameController->IsCellEmpty(cellPosition.x - 1, cellPosition.y, true))
                return new MovementCommand(this, West);
        }

        if ( cellPosition.y < targetPosition.y )
        {
            if (gameController->IsCellEmpty(cellPosition.x, cellPosition.y + 1, true))
                return new MovementCommand(this, South);
        }
        else if ( cellPosition.y > targetPosition.y )
        {
            if (gameController->IsCellEmpty(cellPosition.x, cellPosition.y - 1, true))
                return new MovementCommand(this, North);
        }
    }

    return new WaitCommand(this);
}

void Unit::GiveFood( int32_t food )
{
    this->food += food;
    if (this->food >= MAX_FOOD)
        this->food = MAX_FOOD;
    if ( food < 0 )
        food = 0;
}

void Unit::InflictSleep()
{
    sleepTimer = 12;
    DisplayAnimation(Sleep);
}

void Unit::InflictImmobilize()
{
    immobilizeTimer = 6;
}

void Unit::InflictPoison()
{
    poisonTimer = 8;
}

bool Unit::IsImmobilized()
{
    return immobilizeTimer > 0;
}

bool Unit::IsPoisoned()
{
    return poisonTimer > 0;
}

bool Unit::HasTrapNearby()
{
    Point16 pos = GetGridPosition();
    return gameController->IsTrap(pos.x - 1, pos.y) || gameController->IsTrap(pos.x + 1, pos.y) ||
           gameController->IsTrap(pos.x, pos.y - 1) || gameController->IsTrap(pos.x, pos.y + 1);
}

void Unit::UseItem( ItemType item )
{
    itemToUse = item;
}

int Unit::GetExperience()
{
    return experience;
}

int Unit::GetNeededExperience()
{
    return (level * level / 3) * 6 + 20;
}

void Unit::ChangeAttackEffect( AttackEffect attackEffect )
{
    this->attackEffect = attackEffect;
}

AttackEffect Unit::GetAttackEffect()
{
    return attackEffect;
}

void Unit::UpgradeRandom()
{
    static const int SPECIAL_EFFECT_CHANCE = 25;
    static const int STAT_DECREASE_CHANCE = 5;
    static const int STAT_INCREASE_CHANCE = 80;

    if ( Random(0, 100) < SPECIAL_EFFECT_CHANCE ) // 10% of having a random special attack effect.
        attackEffect = (AttackEffect)Random(0, Attack_Effect_Count);

    int statIncreaseMagnitude = 1 + (numberOfUpgrades / 15);
    int statDecreaseMagnitude = 1 + (numberOfUpgrades / 30); // As you progress with upgrades you might get a downgrade instead.

    // Roll a random for every stat we have.
    float* stats[6] = { &physicalAttack, &physicalDefense, &magicalAttack, &magicalDefense, &maxHealthPoints, &maxMagicPoints };

    for (int i = 0; i < 6; i++)
    {
        if ( Random(0, 100) < STAT_DECREASE_CHANCE )
            *stats[i] -= statDecreaseMagnitude;
        if ( Random(0, 100) < STAT_INCREASE_CHANCE )
            *stats[i] += statIncreaseMagnitude;

        if ( *stats[i] < 0 )
            *stats[i] = 0;
        if ( *stats[i] > 999 )
            *stats[i] = 999;
    }

    numberOfUpgrades++;
}

int Unit::GetUpgradeCost()
{
    return 50 + numberOfUpgrades * 10 + (numberOfUpgrades * numberOfUpgrades / 10) * 2;
}

int Unit::GetLevel()
{
    return level;
}

UnitType Unit::GetUnitType()
{
    return unitType;
}
