#include "GameObjectFactory.h"

#include "Unit.h"
#include "Item.h"

bool GameObjectFactory::isInitialized = false;
UnitDescription GameObjectFactory::unitsDescription[UnitType_Count];
ItemDescription GameObjectFactory::itemsDescription[ItemType_Count];

void GameObjectFactory::InitializeUnitFactory()
{
    UnitDescription& knight = unitsDescription[UnitType_Knight];
    knight.Name = "Knight";			knight.spriteIdentifier = Sprite_Knight;
    knight.startingHealth = 100;		knight.startingMagic = 15;
    knight.healthGrowth = 5.75f;	knight.magicGrowth = 1.25f;
    knight.physicalAttack = 6.5f;		knight.physicalAttackGrowth = 1.25f;
    knight.physicalDefense = 12.5f;	knight.physicalDefenseGrowth = 3.0f;
    knight.magicalAttack = 0.0f;	knight.magicalAttackGrowth = 0.0f;
    knight.magicalDefense = 2.5f;	knight.magicalDefenseGrowth = 1.25f;
    knight.experienceGiven = 0;

    UnitDescription& mage = unitsDescription[UnitType_Mage];
    mage.Name = "Mage";				mage.spriteIdentifier = Sprite_Mage;
    mage.startingHealth = 85;		mage.startingMagic = 15;
    mage.healthGrowth = 4.75f;		mage.magicGrowth = 1.25f;
    mage.physicalAttack = 6.0f;		mage.physicalAttackGrowth = 1.20f;
    mage.physicalDefense = 7.5f;	mage.physicalDefenseGrowth = 1.90f;
    mage.magicalAttack = 0.0f;		mage.magicalAttackGrowth = 0.0f;
    mage.magicalDefense = 2.5f;		mage.magicalDefenseGrowth = 1.25f;
    mage.experienceGiven = 0;

    UnitDescription& thief = unitsDescription[UnitType_Thief];
    thief.Name = "Thief";			thief.spriteIdentifier = Sprite_Thief;
    thief.startingHealth = 95;		thief.startingMagic = 15;
    thief.healthGrowth = 5.25f;		thief.magicGrowth = 1.25f;
    thief.physicalAttack = 6.5f;	thief.physicalAttackGrowth = 2.25f;
    thief.physicalDefense = 8.5f;	thief.physicalDefenseGrowth = 2.10f;
    thief.magicalAttack = 0.0f;		thief.magicalAttackGrowth = 0.0f;
    thief.magicalDefense = 2.5f;	thief.magicalDefenseGrowth = 1.25f;
    thief.experienceGiven = 0;

    UnitDescription& slime = unitsDescription[UnitType_Blue_Slime];
    slime.Name = "Slime";			slime.spriteIdentifier = Sprite_Slime;
    slime.startingHealth = 20.0f;	slime.startingMagic = 0.0f;
    slime.healthGrowth = 13.5f;		slime.magicGrowth = 0.0f;
    slime.physicalAttack = 3.5f;	slime.physicalAttackGrowth = 2.0f;
    slime.physicalDefense = 3.0f;	slime.physicalDefenseGrowth = 1.5f;
    slime.magicalAttack = 0.0f;		slime.magicalAttackGrowth = 0.0f;
    slime.magicalDefense = 1;		slime.magicalDefenseGrowth = 1;
    slime.droppedItem = ItemType_Food_Slime;
    slime.experienceGiven = 3;

    UnitDescription& greenSlime = unitsDescription[UnitType_Green_Slime] = unitsDescription[UnitType_Blue_Slime];
    greenSlime.Name = "G Slime";		greenSlime.spriteIdentifier = Sprite_Slime_Green;
    greenSlime.experienceGiven = 5;		greenSlime.attackEffect = Attack_Effect_Poison;

    UnitDescription& yellowSlime = unitsDescription[UnitType_Yellow_Slime] = unitsDescription[UnitType_Blue_Slime];
    yellowSlime.Name = "Y Slime";		yellowSlime.spriteIdentifier = Sprite_Slime_Yellow;
    yellowSlime.experienceGiven = 5;	yellowSlime.attackEffect = Attack_Effect_Teleport;

    UnitDescription& wererat = unitsDescription[UnitType_Wererat];
    wererat.Name = "Wererat";			wererat.spriteIdentifier = Sprite_Wererat;
    wererat.startingHealth = 35.0f;		wererat.startingMagic = 15.0f;
    wererat.healthGrowth = 12.0f;		wererat.magicGrowth = 0.0;
    wererat.physicalAttack = 4.75f;		wererat.physicalAttackGrowth = 2.00f;
    wererat.physicalDefense = 5.0f;		wererat.physicalDefenseGrowth = 2.5f;
    wererat.magicalAttack = 0.0f;		wererat.magicalAttackGrowth = 1;
    wererat.magicalDefense = 6.0f;		wererat.magicalDefenseGrowth = 1;
    wererat.droppedItem = ItemType_Food_Rat;
    wererat.experienceGiven = 6;

    UnitDescription& blueRat = unitsDescription[UnitType_Blue_Wererat] = unitsDescription[UnitType_Wererat];
    blueRat.Name = "B Rat";		 blueRat.spriteIdentifier = Sprite_Wererat_Blue;
    blueRat.experienceGiven = 8; blueRat.attackEffect = Attack_Effect_Drain_Mp;

    UnitDescription& redRat = unitsDescription[UnitType_Red_Wererat] = unitsDescription[UnitType_Wererat];
    redRat.Name = "R Rat";		redRat.spriteIdentifier = Sprite_Wererat_Red;
    redRat.experienceGiven = 8;	redRat.attackEffect = Attack_Effect_Drain_Hp;

    UnitDescription& wisp = unitsDescription[UnitType_Fire_Wisp];
    wisp.Name = "Wisp";			wisp.spriteIdentifier = Sprite_Fire_Wisp;
    wisp.startingHealth = 35.0f;	wisp.startingMagic = 15.0f;
    wisp.healthGrowth = 20.0f;		wisp.magicGrowth = 0.0;
    wisp.physicalAttack = 2.75f;	wisp.physicalAttackGrowth = 1.95f;
    wisp.physicalDefense = 6.0f;	wisp.physicalDefenseGrowth = 3.5f;
    wisp.magicalAttack = 0.0f;		wisp.magicalAttackGrowth = 1;
    wisp.magicalDefense = 6.0f;		wisp.magicalDefenseGrowth = 1;
    wisp.droppedItem = ItemType_Food_Wisp;
    wisp.experienceGiven = 12;

    UnitDescription& iceWisp = unitsDescription[UnitType_Ice_Wisp] = unitsDescription[UnitType_Fire_Wisp];
    iceWisp.Name = "I Wisp";		 iceWisp.spriteIdentifier = Sprite_Ice_Wisp;
    iceWisp.experienceGiven = 15; iceWisp.attackEffect = Attack_Effect_Drain_Mp;

    UnitDescription& destructionWisp = unitsDescription[UnitType_Destruction_Wisp] = unitsDescription[UnitType_Fire_Wisp];
    destructionWisp.Name = "D Wisp";		 destructionWisp	.spriteIdentifier = Sprite_Destruction_Wisp;
    destructionWisp.experienceGiven = 20;    destructionWisp.attackEffect = Attack_Effect_Random_Damage;

    UnitDescription& brownWisp = unitsDescription[UnitType_Brown_Wisp] = unitsDescription[UnitType_Fire_Wisp];
    brownWisp.Name = "B Wisp";		 brownWisp.spriteIdentifier = Sprite_Brown_Wisp;
    brownWisp.experienceGiven = 14;  brownWisp.attackEffect = Attack_Effect_Immobilize;

    UnitDescription& skeletonMage = unitsDescription[UnitType_Skeleton_Mage];
    skeletonMage.Name = "Mage Sk";			skeletonMage.spriteIdentifier = Sprite_Skeleton_Mage;
    skeletonMage.startingHealth = 35.0f;	skeletonMage.startingMagic = 15.0f;
    skeletonMage.healthGrowth = 20.0f;		skeletonMage.magicGrowth = 0.0;
    skeletonMage.physicalAttack = 3.75f;	skeletonMage.physicalAttackGrowth = 1.85f;
    skeletonMage.physicalDefense = 6.0f;	skeletonMage.physicalDefenseGrowth = 3.5f;
    skeletonMage.magicalAttack = 0.0f;		skeletonMage.magicalAttackGrowth = 1;
    skeletonMage.magicalDefense = 6.0f;		skeletonMage.magicalDefenseGrowth = 1;
    skeletonMage.droppedItem = ItemType_Food_Skeleton;
    skeletonMage.experienceGiven = 20;

    UnitDescription& skeletonThief = unitsDescription[UnitType_Skeleton_Thief] = unitsDescription[UnitType_Skeleton_Mage];
    skeletonThief.Name = "Thief Sk";		skeletonThief.spriteIdentifier = Sprite_Skeleton_Thief;
    skeletonThief.experienceGiven = 20;		skeletonThief.attackEffect = Attack_Effect_Poison;

    UnitDescription& skeletonWarrior = unitsDescription[UnitType_Skeleton_Warrior] = unitsDescription[UnitType_Skeleton_Mage];
    skeletonWarrior.Name = "Knight Sk";		skeletonWarrior.spriteIdentifier = Sprite_Skeleton_Warrior;
    skeletonWarrior.experienceGiven = 20;	skeletonWarrior.attackEffect = Attack_Effect_Sleep;

    UnitDescription& shadow = unitsDescription[UnitType_Shadow];
    shadow.Name = "Shadow";				shadow.spriteIdentifier = Sprite_Shadow;
    shadow.startingHealth = 35.0f;		shadow.startingMagic = 15.0f;
    shadow.healthGrowth = 25.0f;		shadow.magicGrowth = 0.0;
    shadow.physicalAttack = 4.75f;		shadow.physicalAttackGrowth = 2.05f;
    shadow.physicalDefense = 9.0f;		shadow.physicalDefenseGrowth = 6.05f;
    shadow.magicalAttack = 0.0f;		shadow.magicalAttackGrowth = 1;
    shadow.magicalDefense = 6.0f;		shadow.magicalDefenseGrowth = 1;
    shadow.droppedItem = ItemType_AttackEffectPotion; shadow.attackEffect = Attack_Effect_Drain_Hp;
    shadow.experienceGiven = 20;

    UnitDescription& wyvern = unitsDescription[UnitType_Wyvern];
    wyvern.Name = "Wyvern";				wyvern.spriteIdentifier = Sprite_Wyvern;
    wyvern.startingHealth = 50.0f;		wyvern.startingMagic = 15.0f;
    wyvern.healthGrowth = 30.0f;		wyvern.magicGrowth = 0.0;
    wyvern.physicalAttack = 2.75f;		wyvern.physicalAttackGrowth = 1.05f;
    wyvern.physicalDefense = 5.0f;		wyvern.physicalDefenseGrowth = 12.05f;
    wyvern.magicalAttack = 0.0f;		wyvern.magicalAttackGrowth = 1;
    wyvern.magicalDefense = 6.0f;		wyvern.magicalDefenseGrowth = 1;
    wyvern.droppedItem = ItemType_Stairs; wyvern.attackEffect = Attack_Effect_Poison;
    wyvern.experienceGiven = 30;
}

void GameObjectFactory::InitializeItemFactory()
{
    itemsDescription[ItemType_Stairs] = ItemDescription(Sprite_Stairs, Effect_Next_Level, "You walk onto the stairs..", false, true, 0);

    itemsDescription[ItemType_Small_Hp_Potion] = ItemDescription(Sprite_Hp_Potion_Small, Effect_Heal_Hp, "You get healed by %d", true, false, 30);
    itemsDescription[ItemType_Medium_Hp_Potion] = ItemDescription(Sprite_Hp_Potion_Medium, Effect_Heal_Hp, "You get healed by %d", true, false, 75);
    itemsDescription[ItemType_Big_Hp_Potion] = ItemDescription(Sprite_Hp_Potion_Big, Effect_Heal_Hp, "You get healed by %d", true, false, 200);

    itemsDescription[ItemType_Small_Mp_Potion] = ItemDescription(Sprite_Mp_Potion_Small, Effect_Heal_Mp, "Mana restored by %d", true, false, 30);
    itemsDescription[ItemType_Medium_Mp_Potion] = ItemDescription(Sprite_Mp_Potion_Medium, Effect_Heal_Mp, "Mana restored by %d", true, false, 75);
    itemsDescription[ItemType_Big_Mp_Potion] = ItemDescription(Sprite_Mp_Potion_Big, Effect_Heal_Mp, "Mana restored by %d", true, false, 200);

    itemsDescription[ItemType_Sleep_Potion] = ItemDescription(Sprite_Potion_Candy, Effect_Sleep, "Ouch! You fell asleep", true, false, 0);
    itemsDescription[ItemType_Poison_Potion] = ItemDescription(Sprite_Potion_Candy, Effect_Poison, "Ouch! You got poisoned", true, false, 0);

    itemsDescription[ItemType_Teleport_Potion] = ItemDescription(Sprite_Potion_Candy, Effect_Teleport, "Woops! Teleport!", true, false, 0);
    itemsDescription[ItemType_MysteriousPotion] = ItemDescription(Sprite_Potion_Candy, Effect_Random, "Something happened!", true, false, 0);
    itemsDescription[ItemType_GrowPotion] = ItemDescription(Sprite_Potion_Yellow, Effect_Upgrade, "You are stronger!", true, false, 0);
    itemsDescription[ItemType_AttackEffectPotion] = ItemDescription(Sprite_Potion_Pink, Effect_GiveAttackEffect, "Your attack has changed", true, false, 0);
    itemsDescription[ItemType_ExperiencePotion] = ItemDescription(Sprite_Potion_Orange, Effect_GiveExperience, "You gain experience!", true, false, 0);

    itemsDescription[ItemType_Food_Slime] = ItemDescription(Sprite_Food_Slime, Effect_Food, "You eat and feel less hungry", true, true, 15);
    itemsDescription[ItemType_Food_Slime_Exp] = ItemDescription(Sprite_Food_Slime, Effect_GiveExperience, "You gain experience!", true, true, 0);
    itemsDescription[ItemType_Food_Rat] = ItemDescription(Sprite_Food_Wererat, Effect_Food, "You eat and feel less hungry", true, true, 30);
    itemsDescription[ItemType_Food_Rat_Mystery] = ItemDescription(Sprite_Food_Wererat, Effect_Random, "You eat and feel less hungry", true, true, 0);
    itemsDescription[ItemType_Food_Skeleton] = ItemDescription(Sprite_Food_Skeleton, Effect_Food, "You eat and feel less hungry", true, true, 35);
    itemsDescription[ItemType_Food_Wisp] = ItemDescription(Sprite_Food_Fire_Wisp, Effect_Food, "You eat and feel less hungry", true, true, 40);

    itemsDescription[ItemType_Gold_Small] = ItemDescription(Sprite_Gold, Effect_Gold, "You get %d gold!", true, true, 75);
    itemsDescription[ItemType_Gold_Medium] = ItemDescription(Sprite_Gold, Effect_Gold, "You get %d gold!", true, true, 150);
    itemsDescription[ItemType_Gold_Big] = ItemDescription(Sprite_Gold, Effect_Gold, "You get %d gold!", true, true, 300);
    itemsDescription[ItemType_Gold_VeryBig] = ItemDescription(Sprite_Gold, Effect_Gold, "You get %d gold!", true, true, 700);

    itemsDescription[ItemType_Trap_Spikes] = ItemDescription(Sprite_Spikes, Effect_Damage_Hp, "You get damaged by a spike!", false, false, -8);
    itemsDescription[ItemType_Trap_Immobilize] = ItemDescription(Sprite_Trap, Effect_Immobilize, "You are immobilized!", false, false, 0);
    itemsDescription[ItemType_Trap_Sleep] = ItemDescription(Sprite_Trap, Effect_Sleep, "You fell asleep!", false, false, 0);
    itemsDescription[ItemType_Trap_Teleport] = ItemDescription(Sprite_Trap, Effect_Teleport, "You are teleported!", false, false, 0);
    itemsDescription[ItemType_Trap_Poison] = ItemDescription(Sprite_Trap, Effect_Poison, "You are poisoned!", false, false, 0);
}

Unit* GameObjectFactory::CreateUnit( GameController* controller, UnitType type )
{
    if ( isInitialized == false )
    {
        InitializeUnitFactory();
        InitializeItemFactory();
        isInitialized = true;
    }
    Unit* unit = new Unit(type, controller);
    unit->DisplayAnimation(Idle);
    return unit;
}

Item* GameObjectFactory::CreateItem( GameController* controller, ItemType type )
{
    if ( isInitialized == false )
    {
        InitializeUnitFactory();
        InitializeItemFactory();
        isInitialized = true;
    }
    Item* item = new Item(controller, type, itemsDescription[type]);
    item->DisplayAnimation(Still);
    return item;
}

const UnitDescription& GameObjectFactory::GetUnitDescription( UnitType type )
{
    if ( isInitialized == false )
    {
        InitializeUnitFactory();
        InitializeItemFactory();
        isInitialized = true;
    }
    return unitsDescription[type];
}

