/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   18:53
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\Command.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	Command
	file ext:	h
	author:		Stefano Musumeci

	purpose:	Base class for all the actions executed in game.
				Entry point for the logic of actions such as attack, move, wait and use item.
*********************************************************************/
#ifndef Command_h__
#define Command_h__

enum CommandState
{
    Executing,
    Done,
};

class Command
{
    public:
        virtual CommandState Execute() = 0;
        virtual ~Command() { }
};

#endif // Command_h__
