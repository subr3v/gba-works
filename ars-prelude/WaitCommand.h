/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   18:54
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\WaitCommand.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	WaitCommand
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef WaitCommand_h__
#define WaitCommand_h__

#include "Command.h"
#include "Unit.h"

class WaitCommand : public Command
{
    public:
        WaitCommand(Unit* unit) : Command()
        {
            this->unit = unit;
        }
        virtual CommandState Execute()
        {
            if ( unit->GetFood() >= 20 )
                unit->ModifyHealth(1);
            unit->GiveFood(-1);
            return Done;
        }

    private:
        Unit* unit;
};

#endif // WaitCommand_h__