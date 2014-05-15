#include "DungeonLevel.h"

bool DungeonLevel::levelsInitialized = false;
DungeonLevel DungeonLevel::levels[DUNGEON_LEVELS_COUNT];

const DungeonLevel& DungeonLevel::GetLevel( int number )
{
    if (levelsInitialized == false)
    {
        InitializeLevels();
        levelsInitialized = true;
    }
    return levels[number];
}

const int DungeonLevel::GetNumberOfLevels()
{
    return DUNGEON_LEVELS_COUNT;
}

void DungeonLevel::InitializeLevels()
{
    int levelWidth = 48;
    int levelHeight = 48;
    int roomCount = 5;
    int monstersLevel = 1;
    int monstersCount = 2;
    int treasuresCount = 1;
    int trapsCount = 0;
    int itemsCount = 1;

    static const int SIZE_INCREASE = 6;
    static const int MAX_ROOMS = 16;
    static const int MAX_ITEMS = 4;
    static const int MAX_MONSTERS = 6;
    static const int MAX_TRAPS = 3;
    static const int MAX_TREASURES = 3;

    for (int i = 0; i < DUNGEON_LEVELS_COUNT; i++)
    {
        DungeonSetting setting;

        int levelNum = i + 1;
        if ( levelNum % 5 == 0 )
            setting = Volcano;
        else if ( levelNum < 10 )
            setting = Castle;
        else if ( levelNum < 20 )
            setting = Undersea;

        levels[i] = DungeonLevel(setting, levelWidth, levelHeight, roomCount, monstersLevel, monstersCount, treasuresCount, trapsCount, itemsCount);

        if ( levelWidth + SIZE_INCREASE < MAX_MAP_SIZE )
        {
            levelWidth += SIZE_INCREASE;
            levelHeight += SIZE_INCREASE;
        }

        if ( itemsCount < MAX_ITEMS && i % 12 == 0 )
            itemsCount++;

        if ( monstersCount < MAX_MONSTERS && i % 3 == 0 )
            monstersCount ++;

        if ( roomCount < MAX_ROOMS && i % 5 == 0 )
            roomCount++;

        monstersLevel++;

        if ( trapsCount < MAX_TRAPS && i % 10 == 0 )
            trapsCount++;

        if ( treasuresCount < MAX_TREASURES && i % 10 == 0)
            treasuresCount++;
    }
}
