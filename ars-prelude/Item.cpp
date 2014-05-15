#include "Item.h"
#include <cstdio>
#include "GameController.h"

Item::Item( GameController* gameController, ItemType type, const ItemDescription& itemDescription ) : GameObject(gameController, Object_Item)
    , description(itemDescription)
{
    this->SetSpriteIdentifier(itemDescription.spriteIdentifier);
    this->itemType = type;
    this->showForeground = false;
}

void Item::OnStep( Unit* steppedUnit )
{
    GameController* gameController = steppedUnit->GetGameController();

    if ( steppedUnit->IsHumanControlled() == false && description.isHumanOnly)
        return;

    ItemEffect effect = description.effect;
    if ( effect == Effect_Random )
    {
        effect = (ItemEffect)Random(Effect_Teleport, Effect_Random);
    }

    switch (effect)
    {
        case Effect_Next_Level:
            {
                gameController->MoveToNextLevel();
                break;
            }
        case Effect_Food:
            {
                steppedUnit->GiveFood(description.itemParameter);
                break;
            }
        case Effect_Gold:
            {
                steppedUnit->ModifyGold(description.itemParameter);
                break;
            }
        case Effect_Heal_Hp:
        case Effect_Damage_Hp:
            {
                steppedUnit->ModifyHealth(description.itemParameter);
                break;
            }
        case Effect_Heal_Mp:
        case Effect_Damage_Mp:
            {
                steppedUnit->ModifyMagic(description.itemParameter);
                break;
            }
        case Effect_Teleport:
            {
                steppedUnit->position = gameController->GetRandomPositionWorld(false);
                break;
            }
        case Effect_Sleep:
            {
                steppedUnit->InflictSleep();
                break;
            }
        case Effect_Poison:
            {
                steppedUnit->InflictPoison();
                break;
            }
        case Effect_Immobilize:
            {
                steppedUnit->InflictImmobilize();
                break;
            }
        case Effect_GiveAttackEffect:
            {
                steppedUnit->ChangeAttackEffect((AttackEffect)Random(0, Attack_Effect_Count));
                break;
            }
        case Effect_GiveExperience:
            {
                steppedUnit->GiveExperience(steppedUnit->GetNeededExperience() / 2);
                break;
            }
        case Effect_Upgrade:
            {
                steppedUnit->UpgradeRandom();
                break;
            }
        default:
            break;
    }

    if ( steppedUnit->IsHumanControlled())
    {
        gameController->ShowText(description.message, description.itemParameter);
    }

    this->DisplayAnimation(TrapTrigger);

    if (description.autoDestroy)
        this->Kill();
}

ItemType Item::GetItemType()
{
    return itemType;
}

void Item::EnableShowForeground( bool enable )
{
    this->showForeground = enable;
}

bool Item::IsShowForeground()
{
    return showForeground;
}

bool Item::IsTrap()
{
    return itemType >= ItemType_Trap_Spikes && itemType <= ItemType_Trap_Teleport;
}
