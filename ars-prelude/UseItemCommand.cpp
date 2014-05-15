#include "UseItemCommand.h"
#include "GameController.h"
#include "Unit.h"
#include "Item.h"
#include "GameObjectFactory.h"

UseItemCommand::UseItemCommand( Unit* unit, ItemType itemType )
{
    GameController* gameController = unit->GetGameController();
    this->unit = unit;
    this->usedItem = GameObjectFactory::CreateItem(gameController, itemType);
    this->unit->currentDirection = South;
    this->unit->DisplayAnimation(Special);

    this->usedItem->position = unit->position;
    this->usedItem->position.y += 6; // Spawn the item 6 pixels down from the player and then animate it so that it moves upwards

    this->usedItem->EnableShowForeground(true); // This particular item is supposed to be drawn after the player.
    gameController->AddObject(usedItem);

    this->frameCounter = 0;
}

CommandState UseItemCommand::Execute()
{
    frameCounter++;

    if ( frameCounter % 8 == 0 )
    {
        this->usedItem->position.y--;

        if ( this->usedItem->position.y == unit->position.y )
        {
            this->usedItem->OnStep(unit);
            this->usedItem->Kill();
            this->unit->DisplayAnimation(Idle);
            return Done;
        }
        frameCounter = 0;
    }

    return Executing;
}