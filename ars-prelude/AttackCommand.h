/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   18:54
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\AttackCommand.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	AttackCommand
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef AttackCommand_h__
#define AttackCommand_h__

#include "Command.h"
#include "GameConstants.h"

class Unit;

class AttackCommand : public Command
{
    public:
        AttackCommand(Unit* attackingUnit, Direction attackDirection);
        virtual CommandState Execute();
    private:
        Unit* unit;
        Unit* hitUnit;
        Direction attackDirection;

        int counter;
        int attackCounter;

        enum AttackAnimationState
        {
            AttackPhase,
            RetreatPhase,
            End
        };

        AttackAnimationState currentState;
};

#endif // AttackCommand_h__
