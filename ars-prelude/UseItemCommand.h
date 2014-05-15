/********************************************************************
	created:	2014/04/10
	created:	10:4:2014   23:26
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\UseItemCommand.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	UseItemCommand
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef UseItemCommand_h__
#define UseItemCommand_h__

#include "Command.h"
#include "GameConstants.h"

class Unit;
class Item;

class UseItemCommand : public Command
{
    public:
        UseItemCommand(Unit* unit, ItemType itemType);
        virtual CommandState Execute();
    private:
        int frameCounter;
        Unit* unit;
        Item* usedItem;
};


#endif // UseItemCommand_h__
