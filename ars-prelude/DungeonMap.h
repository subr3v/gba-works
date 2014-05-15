/********************************************************************
	created:	2014/03/06
	created:	6:3:2014   15:00
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\Dungeon.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	Dungeon
	file ext:	h
	author:		Stefano Musumeci

	purpose:	Dungeon class
*********************************************************************/
#ifndef Dungeon_h__
#define Dungeon_h__

#include <stdint.h>

class DungeonGenerator;

enum Tile
{
    Empty,
    Wall,
    Floor,
    Corridor,
    Door,
    DoorWalker,
};

class DungeonMap
{
    public:
        DungeonMap();
        ~DungeonMap();

        void Resize(int width, int height);
        void Clear();
        // Generates a 8x8 graphical tilemap and put all the data inside the array tileData.
        void GenerateTilemap24(short* tileData);

        uint16_t GetTile(int x, int y)
        {
            if ( x < 0 || y < 0 || y >= height || x >= width)
                return -1; // It's actually the highest number representable with an unsigned short (0xFFFF).
            return mapData[y * width + x];
        }

        bool IsTile(int x, int y, short tileType);
        bool IsTileNot(int x, int y, short tileType);

        bool IsFloor(int x, int y);
        bool IsWall(int x, int y);
        bool IsDoor(int x, int y);
        bool IsEmpty(int x, int y);
        bool IsCorridor(int x, int y);
        bool IsWalkable(int x, int y);

        int32_t GetWidth() { return width; }
        int32_t GetHeight() { return height; }
    private:
        friend class DungeonGenerator;
        void SetTile(int x, int y, short tile) { mapData[y * width + x] = tile; }

        short* mapData;
        int width, height;
};

#endif // Dungeon_h__
