/********************************************************************
	created:	2014/03/29
	created:	29:3:2014   17:22
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\DungeonGenerator.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	DungeonGenerator
	file ext:	h
	author:		Stefano Musumeci

	purpose:	Dungeon generator class
*********************************************************************/
#ifndef DungeonGenerator_h__
#define DungeonGenerator_h__

#include "stdint.h"
#include "GameConstants.h"

class DungeonMap;

class DungeonGenerator
{
    public:
        DungeonGenerator(DungeonMap* map);
        ~DungeonGenerator();
        void GenerateDungeon(int32_t width, int32_t height, int32_t roomNumber);
    private:
        DungeonMap* map;
        int generatedRooms;

        // Returns a direction that leads to an empty tile
        Direction GetFreeDirection(int32_t x, int32_t y);
        // Execute the walker logic (ie. move by a random number of tiles starting from a direction)
        void DoWalker(int32_t& x, int32_t& y, Direction& walkDirection);
        // Execute a step in a specific direction and returns the new coordinates
        bool DoStep(int32_t& x, int32_t& y, Direction stepDirection);
        bool CreateRoom(int32_t x, int32_t y, int32_t w, int32_t h);
        // Returns true if the area is full of empty tiles
        bool IsAreaEmpty(int32_t x, int32_t y, int32_t w, int32_t h);

        static const int MAX_CREATE_WALKER_STEP_ITERATIONS = 20;
        static const int MAX_CREATE_ROOM_ITERATIONS = 20;
        static const int MAX_CREATE_ROOM_WALKER_ITERATIONS = 30;
        static const int MINIMUM_ROOM_SIZE = 3;
        static const int MAXIMUM_ROOM_SIZE = 10;

        static const int MINIMUM_WALKER_ROOM = 2;
        static const int MAXIMUM_WALKER_ROOM = 3;

        static const int MINIMUM_STEPS_PER_WALKER = 6;
        static const int MAXIMUM_STEPS_PER_WALKER = 12;

        static const int MINIMUM_STEPS_DIRECTION = 2;
        static const int MAXIMUM_STEPS_DIRECTION = 4;
};


#endif // DungeonGenerator_h__
