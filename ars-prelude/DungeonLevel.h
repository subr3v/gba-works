/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   20:08
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\DungeonLevel.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	DungeonLevel
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef DungeonLevel_h__
#define DungeonLevel_h__

#include "stdint.h"
#include "GameConstants.h"

struct DungeonLevel
{
    public:
        uint16_t width;
        uint16_t height;
        uint16_t roomNumber;

        uint16_t monstersCount;
        uint16_t monstersLevel;
        uint16_t treasuresCount;
        uint16_t trapsCount;
        uint16_t itemsCount;

        DungeonSetting setting;

        DungeonLevel() { }

        DungeonLevel(DungeonSetting setting, uint16_t width, uint16_t height, uint16_t roomNumber, uint16_t monstersLevel, uint16_t monstersCount, uint16_t treasuresCount, uint16_t trapsCount, uint16_t itemsCount)
        {
            this->width = width;
            this->height = height;
            this->roomNumber = roomNumber;
            this->monstersLevel = monstersLevel;
            this->monstersCount = monstersCount;
            this->trapsCount = trapsCount;
            this->treasuresCount = treasuresCount;
            this->itemsCount = itemsCount;
            this->setting = setting;
        }

        static const int GetNumberOfLevels();
        static const DungeonLevel& GetLevel(int number);
    private:
        static void InitializeLevels();
        static bool levelsInitialized;
        static const int DUNGEON_LEVELS_COUNT = 20;
        static DungeonLevel levels[DUNGEON_LEVELS_COUNT];
};

#endif // DungeonLevel_h__
