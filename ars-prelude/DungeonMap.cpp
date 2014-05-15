#include "DungeonMap.h"
#include "System.h"
#include <cstring>
#include <vector>
#include <queue>
#include <map>
#include "DungeonGenerator.h"

DungeonMap::DungeonMap()
{
    this->width = this->height = 0;
    this->mapData = NULL;
}

DungeonMap::~DungeonMap()
{
    delete[] mapData;
}

bool DungeonMap::IsTile( int x, int y, short tileType )
{
    if ( x < 0 || x >= width || y < 0 || y >= height )
        return false;
    return GetTile(x, y) == tileType;
}

bool DungeonMap::IsWalkable( int x, int y )
{
    if ( x < 0 || x >= width || y < 0 || y >= height )
        return false;
    uint16_t tile = GetTile(x, y);
    return tile == Door || tile == Corridor || tile == Floor;
}

bool DungeonMap::IsDoor( int x, int y )
{
    return IsTile(x, y, Door);
}

bool DungeonMap::IsEmpty( int x, int y )
{
    return IsTile(x, y, Empty);
}

bool DungeonMap::IsTileNot( int x, int y, short tileType )
{
    if ( x < 0 || x >= width || y < 0 || y >= height )
        return false;
    return GetTile(x, y) != tileType;
}

bool DungeonMap::IsWall( int x, int y )
{
    return IsTile(x, y, Wall);
}

bool DungeonMap::IsCorridor( int x, int y )
{
    return IsTile(x, y, Corridor);
}

bool DungeonMap::IsFloor( int x, int y )
{
    return IsTile(x, y, Floor);
}

void DungeonMap::Resize( int width, int height )
{
    this->width = width;
    this->height = height;

    if ( mapData != NULL )
        delete[] mapData;

    mapData = new short[width * height];
}

enum AdjacencyInformation
{
    None = 0,
    Right = 1 << 0,
    Left = 1 << 1,
    Up = 1 << 2,
    Down = 1 << 3,
    SouthRight = 1 << 4,
    SouthLeft = 1 << 5,
    NorthRight = 1 << 6,
    NorthLeft = 1 << 7,
};

void DungeonMap::GenerateTilemap24(short* tileData)
{
    // This function converts logical tiles into graphical tiles.
    int tilemapWidth  = width * 3;

    std::map<int, int> terrainAutomap;
    std::map<int, int> wallAutomap;
    std::map<int, int> emptyAutomap;

    // Ground tiles as found in reference tileset.
    const int GROUND_NW = TileID<0, 0>::Value;
    const int GROUND_N = TileID<1, 0>::Value;
    const int GROUND_NE = TileID<2, 0>::Value;
    const int GROUND_E = TileID<0, 1>::Value;
    const int GROUND_C = TileID<1, 1>::Value;
    const int GROUND_C_1 = TileID<2, 3>::Value;
    const int GROUND_W = TileID<2, 1>::Value;
    const int GROUND_SW = TileID<0, 2>::Value;
    const int GROUND_S = TileID<1, 2>::Value;
    const int GROUND_SE = TileID<2, 2>::Value;
    const int GROUND_C_NE = TileID<3, 0>::Value;
    const int GROUND_C_NW = TileID<4, 0>::Value;
    const int GROUND_C_SW = TileID<3, 1>::Value;
    const int GROUND_C_SE = TileID<3, 2>::Value;

    const int GROUND_C_N = TileID<3, 3>::Value;
    const int GROUND_C_S = TileID<4, 3>::Value;
    const int GROUND_C_E = TileID<4, 2>::Value;
    const int GROUND_C_W = TileID<4, 1>::Value;

    const int WALL_1 = TileID<0, 3>::Value;
    const int WALL_2 = TileID<1, 3>::Value;

    const int BLANK_TILE = TileID<0, 7>::Value;
    const int EMPTY_NW = TileID<0, 4>::Value;
    const int EMPTY_N = TileID<1, 4>::Value;
    const int EMPTY_NE = TileID<2, 4>::Value;
    const int EMPTY_E = TileID<0, 5>::Value;
    const int EMPTY_W = TileID<2, 5>::Value;
    const int EMPTY_SW = TileID<0, 6>::Value;
    const int EMPTY_S = TileID<1, 6>::Value;
    const int EMPTY_SE = TileID<2, 6>::Value;
    const int EMPTY_C_SE = TileID<3, 4>::Value;
    const int EMPTY_C_SW = TileID<4, 4>::Value;
    const int EMPTY_C_NE = TileID<3, 5>::Value;
    const int EMPTY_C_NW = TileID<4, 5>::Value;

    // Main floor terrain cases
    terrainAutomap[None] = GROUND_C;
    terrainAutomap[Up] = GROUND_N;
    terrainAutomap[Up | Left] = GROUND_NW;
    terrainAutomap[Up | Right] = GROUND_NE;
    terrainAutomap[Right] = GROUND_W;
    terrainAutomap[Left] = GROUND_E;
    terrainAutomap[Down] = GROUND_S;
    terrainAutomap[Down | Right] = GROUND_SE;
    terrainAutomap[Down | Left] = GROUND_SW;

    // Corridor cases.
    terrainAutomap[Up | Down] = GROUND_N;
    terrainAutomap[Right | Left] = GROUND_C;

    // Corner cases.
    terrainAutomap[NorthLeft] = GROUND_C_NW;
    terrainAutomap[NorthRight] = GROUND_C_NE;
    terrainAutomap[SouthLeft] = GROUND_C_SW;
    terrainAutomap[SouthRight] = GROUND_C_SE;

    terrainAutomap[NorthLeft | NorthRight] = GROUND_C_N;
    terrainAutomap[SouthLeft | SouthRight] = GROUND_C_S;
    terrainAutomap[SouthLeft | NorthLeft] = GROUND_C_W;
    terrainAutomap[SouthRight | NorthRight] = GROUND_C_E;

    wallAutomap[Down] = WALL_1;
    wallAutomap[Down | Up] = WALL_1;
    wallAutomap[Down | Right] = WALL_1;
    wallAutomap[Down | Right | Up] = WALL_1;
    wallAutomap[Down | Left] = WALL_1;
    wallAutomap[Down | Left | Up] = WALL_1;
    wallAutomap[Down | Left | Right] = WALL_1;
    wallAutomap[Down | Left | Right | Up] = WALL_1;

    emptyAutomap[None] = BLANK_TILE;
    emptyAutomap[Right] = EMPTY_W;
    emptyAutomap[Left] = EMPTY_E;
    emptyAutomap[Up] = EMPTY_N;
    emptyAutomap[Down] = EMPTY_S;
    emptyAutomap[Up | Left] = EMPTY_NW;
    emptyAutomap[Up | Right] = EMPTY_NE;
    emptyAutomap[Down | Right] = EMPTY_SE;
    emptyAutomap[Down | Left] = EMPTY_SW;

    // Corner cases.
    emptyAutomap[NorthLeft] = EMPTY_C_NW;
    emptyAutomap[NorthRight] = EMPTY_C_NE;
    emptyAutomap[SouthLeft] = EMPTY_C_SW;
    emptyAutomap[SouthRight] = EMPTY_C_SE;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            uint16_t tileId = BLANK_TILE; // defaults to blank tile

            if ( IsTile(x, y, Wall) )
            {
                uint16_t wallAdjacency = None;

                if ( IsWalkable(x - 1, y)) { wallAdjacency |= Left; }
                if ( IsWalkable(x + 1, y)) { wallAdjacency |= Right; }
                if ( IsWalkable(x, y - 1)) { wallAdjacency |= Up; }
                if ( IsWalkable(x, y + 1)) { wallAdjacency |= Down; }

                if ( wallAutomap[wallAdjacency] != 0 )
                {
                    tileId = wallAutomap[wallAdjacency];
                }
                else
                {
                    tileId = WALL_2;
                }
            }
            else if ( IsEmpty(x, y) == false )
            {
                uint16_t adjacencyInfo = None;

                if ( IsTile(x - 1, y, Wall)) { adjacencyInfo |= Left; }
                if ( IsTile(x + 1, y, Wall)) { adjacencyInfo |= Right; }
                if ( IsTile(x, y - 1, Wall)) { adjacencyInfo |= Up; }
                if ( IsTile(x, y + 1, Wall)) { adjacencyInfo |= Down; }

                if ( adjacencyInfo == None )
                {
                    if ( IsTile(x + 1, y + 1, Wall)) { adjacencyInfo |= SouthRight; }
                    if ( IsTile(x - 1, y + 1, Wall)) { adjacencyInfo |= SouthLeft; }
                    if ( IsTile(x + 1, y - 1, Wall)) { adjacencyInfo |= NorthRight; }
                    if ( IsTile(x - 1, y - 1, Wall)) { adjacencyInfo |= NorthLeft; }
                }

                tileId = terrainAutomap[adjacencyInfo];
                if ( tileId == GROUND_C && Random(0, 100) < 50 ) // Randomize with alternative ground tile to make it more interesting.
                    tileId = GROUND_C_1;
            }
            else
            {
                uint16_t adjacencyInfo = None;
                if ( IsTile(x - 1, y, Wall)) { adjacencyInfo |= Left; }
                if ( IsTile(x + 1, y, Wall)) { adjacencyInfo |= Right; }
                if ( IsTile(x, y - 1, Wall)) { adjacencyInfo |= Up; }
                if ( IsTile(x, y + 1, Wall)) { adjacencyInfo |= Down; }

                if ( adjacencyInfo == None )
                {
                    if ( IsTile(x + 1, y + 1, Wall)) { adjacencyInfo |= SouthRight; }
                    if ( IsTile(x - 1, y + 1, Wall)) { adjacencyInfo |= SouthLeft; }
                    if ( IsTile(x + 1, y - 1, Wall)) { adjacencyInfo |= NorthRight; }
                    if ( IsTile(x - 1, y - 1, Wall)) { adjacencyInfo |= NorthLeft; }
                }

                tileId = emptyAutomap[adjacencyInfo];
            }

            tileId *= 3; // Convert to 24 x 24 offset
            int mapX = x * 3;
            int mapY = y * 3;

            tileData[(mapY + 0) * tilemapWidth + (mapX + 0)] = BG_TILE(tileId, 0, false, false);
            tileData[(mapY + 0) * tilemapWidth + (mapX + 1)] = BG_TILE(tileId + 1, 0, false, false);
            tileData[(mapY + 0) * tilemapWidth + (mapX + 2)] = BG_TILE(tileId + 2, 0, false, false);
            tileData[(mapY + 1) * tilemapWidth + (mapX + 0)] = BG_TILE(tileId + 16, 0, false, false);
            tileData[(mapY + 1) * tilemapWidth + (mapX + 1)] = BG_TILE(tileId + 17, 0, false, false);
            tileData[(mapY + 1) * tilemapWidth + (mapX + 2)] = BG_TILE(tileId + 18, 0, false, false);
            tileData[(mapY + 2) * tilemapWidth + (mapX + 0)] = BG_TILE(tileId + 32, 0, false, false);
            tileData[(mapY + 2) * tilemapWidth + (mapX + 1)] = BG_TILE(tileId + 33, 0, false, false);
            tileData[(mapY + 2) * tilemapWidth + (mapX + 2)] = BG_TILE(tileId + 34, 0, false, false);
        }
    }
}

void DungeonMap::Clear()
{
    memset(mapData, 0, sizeof(short) * width * height);
}
