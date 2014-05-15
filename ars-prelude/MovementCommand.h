
/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   18:54
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\MovementCommand.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	MovementCommand
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef MovementCommand_h__
#define MovementCommand_h__

#include "Command.h"
#include "stdint.h"
#include "GameConstants.h"
#include "Unit.h"

class MovementCommand : public Command
{
    public:
        MovementCommand(Unit* unit, Direction movementDirection);
        virtual CommandState Execute();
    private:
        int counter;
        Unit* unit; // Moving unit.
        Direction movementDirection; // Direction in which the unit is moving to.
};

#endif // MovementCommand_h__