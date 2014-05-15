#include "MovementCommand.h"
#include "Item.h"
#include <memory>
#include "GameController.h"

MovementCommand::MovementCommand( Unit* unit, Direction movementDirection )
{
    this->unit = unit;
    this->movementDirection = movementDirection;
    this->counter = 0;
    this->unit->currentDirection = movementDirection;
    this->unit->DisplayAnimation(Walk);

    // If distance from player is more than 25 cells then we skip the animation.
    GameController* gameController = unit->GetGameController();
    Point16 playerPosition = gameController->GetPlayer()->GetGridPosition();
    Point16 unitPosition = unit->GetGridPosition();
    Point16 diff(playerPosition.x - unitPosition.x, playerPosition.y - unitPosition.y);
    int distance = diff.x * diff.x + diff.y * diff.y;
    if ( distance >= 25 )
    {
        counter = TILE_SIZE;
        if ( unit->IsImmobilized() == false )
        {
            unit->Move(movementDirection, TILE_SIZE);
        }
    }
    else
    {
        if ( unit->GetUnitType() == UnitType_Wyvern ) // Special case for the wyvern, that shakes the screen every time it moves (while being in sight).
        {
            gameController->ShakeCamera(2, 10);
        }
    }
}

CommandState MovementCommand::Execute()
{
    if ( counter < TILE_SIZE )
    {
        if ( unit->IsImmobilized() == false )
            unit->Move(movementDirection, 1);

        counter++;
        return Executing;
    }
    else
    {
        unit->DisplayAnimation(Idle);
        // Check if something happens: ie. we stepped in the stairs or a trap.
        // We only check this if we're not immobilized, because otherwise we're obviously on the same tile.
        if ( unit->IsImmobilized() == false)
        {
            Point16 unitPos = unit->GetGridPosition();
            Item* steppedItem = unit->GetGameController()->GetItem(unitPos.x, unitPos.y);
            if (steppedItem != NULL)
                steppedItem->OnStep(unit);
        }
        return Done;
    }
}
