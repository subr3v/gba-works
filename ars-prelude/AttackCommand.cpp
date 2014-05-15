#include "AttackCommand.h"
#include "Unit.h"
#include "GameController.h"

AttackCommand::AttackCommand( Unit* attackingUnit, Direction attackDirection )
{
    this->unit = attackingUnit;
    this->unit->currentDirection = attackDirection;
    this->attackDirection = attackDirection;
    this->counter = 0;
    this->currentState = AttackPhase;
    this->hitUnit = NULL;
    this->attackCounter = 0;
    GameController* gameController = unit->GetGameController();
    hitUnit = gameController->GetUnit(
                  unit->position.x / TILE_SIZE + GetHorizontalComponent(unit->currentDirection),
                  unit->position.y / TILE_SIZE + GetVerticalComponent(unit->currentDirection));
    gameController->EnableCameraFollowObject(false);
}

CommandState AttackCommand::Execute()
{
    const int MOV_SPEED = 1;
    counter += MOV_SPEED;

    switch (currentState)
    {
        case AttackCommand::AttackPhase:
            {
                if ( counter == 3 )
                {
                    this->unit->DisplayAnimation(Attack);
                }

                if ( counter < TILE_SIZE / 2)
                {
                    unit->Move(attackDirection, MOV_SPEED);
                }
                else
                {
                    counter = 0;
                    currentState = RetreatPhase;
                    if ( hitUnit != NULL )
                    {
                        hitUnit->DisplayAnimation(Hurt);
                        unit->DamageUnit(hitUnit);
                    }
                }
            }
            break;
        case AttackCommand::RetreatPhase:
            {
                if ( counter < TILE_SIZE / 2 )
                {
                    unit->Move(attackDirection, -MOV_SPEED);
                }
                else
                {
                    if ( hitUnit != NULL )
                    {
                        hitUnit->DisplayAnimation(Idle);
                    }
                    counter = 0;
                    currentState = End;
                }
            }
            break;
        case AttackCommand::End:
            unit->DisplayAnimation(Idle);
            unit->GetGameController()->EnableCameraFollowObject(true);
            return Done;
    }
    return Executing;
}