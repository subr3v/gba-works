#include "DungeonGenerator.h"
#include "DungeonMap.h"
#include "System.h"
#include <cstring>
#include <vector>
#include "GraphicsUtil.h"

DungeonGenerator::DungeonGenerator( DungeonMap* map )
{
    this->map = map;
}

DungeonGenerator::~DungeonGenerator()
{

}

Direction DungeonGenerator::GetFreeDirection( int32_t x, int32_t y )
{
    if ( DoStep(x, y, East))
        return East;
    if ( DoStep(x, y, South))
        return South;
    if ( DoStep(x, y, North))
        return North;
    if ( DoStep(x, y, West))
        return West;
    return Direction_None;
}

bool DungeonGenerator::DoStep( int32_t& x, int32_t& y, Direction stepDirection )
{
    int xIncrease = GetHorizontalComponent(stepDirection);
    int yIncrease = GetVerticalComponent(stepDirection);
    if ( map->IsEmpty(x + xIncrease, y + yIncrease) )
    {
        x += xIncrease;
        y += yIncrease;
        return true;
    }
    return false;
}

void DungeonGenerator::DoWalker( int32_t& x, int32_t& y, Direction& walkDirection  )
{
    if ( walkDirection == Direction_None ) // Choose a random direction.
    {
        walkDirection = GetRandomDirection();
    }

    int numberOfSteps = Random(MINIMUM_STEPS_PER_WALKER, MAXIMUM_STEPS_PER_WALKER); // Determine how many steps we want this walker to execute
    for (int i = 0; i < MAX_CREATE_WALKER_STEP_ITERATIONS; i++)
    {
        int movementCounter = Random(MINIMUM_STEPS_DIRECTION, MAXIMUM_STEPS_DIRECTION);
        for (int k = 0; k < movementCounter; k++)
        {
            bool successfulStep = DoStep(x, y, walkDirection);
            if ( successfulStep )
                map->SetTile(x, y, Corridor);
            else
                break; // There's no point in trying to walk in a direction which is blocked.
            numberOfSteps--;
        }
        if ( numberOfSteps > 0 ) // Choose another direction.
            walkDirection = GetRandomDirection();
        else
            break; // We covered a satisfying number of steps.
    }
}

struct WalkerInstance
{
    int32_t x;
    int32_t y;
    Direction direction;

    WalkerInstance(int x = 0, int y = 0, Direction direction = Direction_None)
    {
        this->x = x;
        this->y = y;
        this->direction = direction;
    }
};

void DungeonGenerator::GenerateDungeon( int32_t width, int32_t height, int32_t roomNumber )
{
    map->Resize(width, height);

    generatedRooms = 0;
    while (generatedRooms < roomNumber)
    {
        // Reset map and make walls all around it
        map->Clear();
        for (int x = 0; x < width; x++)
        {
            map->SetTile(x, 0, Wall);
            map->SetTile(x, height - 1, Wall);
        }
        for (int y = 0; y < height; y++)
        {
            map->SetTile(0, y, Wall);
            map->SetTile(width - 1, y, Wall);
        }


        std::vector<WalkerInstance> walkers;
        generatedRooms = 0;

        int startX = Random(0, width - 1);
        int startY = Random(0, height - 1);
        Direction startDirection = GetRandomDirection();
        walkers.push_back(WalkerInstance(startX, startY, startDirection));

        while ( generatedRooms < roomNumber && walkers.size() > 0 )
        {
            WalkerInstance walker = walkers[0];
            walkers.erase(walkers.begin());

            DoWalker(walker.x, walker.y, walker.direction);

            int spawnDecision = Random(0, 100);
            if ( spawnDecision >= 80 ) // % we create another walker
            {
                walkers.push_back(WalkerInstance(walker.x, walker.y, walker.direction)); // Add a random walker
            }
            else // otherwise we create a room
            {
                // We create a room "forward" based on the previous direction.
                int roomX = 0;
                int roomY = 0;
                int roomWidth = 0;
                int roomHeight = 0;

                int32_t stepDoorX = walker.x;
                int32_t stepDoorY = walker.y;
                Direction stepDirection = walker.direction;
                bool roomGenerated = false;
                for (int i = 0; i < MAX_CREATE_ROOM_ITERATIONS; i++)
                {
                    stepDoorX = walker.x;
                    stepDoorY = walker.y;
                    if ( DoStep(stepDoorX, stepDoorY, stepDirection) ) // Try walking inside the room
                    {
                        roomWidth = Random(MINIMUM_ROOM_SIZE, MAXIMUM_ROOM_SIZE);
                        roomHeight = Random(MINIMUM_ROOM_SIZE, MAXIMUM_ROOM_SIZE);

                        int horizontalMovement = GetHorizontalComponent(walker.direction);
                        int verticalMovement = GetVerticalComponent(walker.direction);

                        if ( horizontalMovement != 0 )
                        {
                            int yOffset = Random(1, roomHeight - 1);
                            roomX = stepDoorX;
                            roomY = stepDoorY - yOffset;
                            if ( horizontalMovement == -1 )
                                roomX = stepDoorX - roomWidth;
                        }
                        else
                        {
                            int xOffset = Random(1, roomWidth - 1);
                            roomX = stepDoorX - xOffset;
                            roomY = stepDoorY;
                            if ( verticalMovement == -1 )
                                roomY = stepDoorY - roomHeight;
                        }

                        if ( CreateRoom(roomX, roomY, roomWidth, roomHeight) )
                        {
                            roomGenerated = true;
                            generatedRooms ++;
                            break;
                        }
                    }
                    stepDirection = GetRandomDirection(); // We try another random direction.
                }

                if ( roomGenerated )
                {
                    // Create a door for the room
                    map->SetTile(stepDoorX, stepDoorY, Door);

                    int spawnedWalkers = Random(MINIMUM_WALKER_ROOM, MAXIMUM_WALKER_ROOM);

                    for (int k = 0; k < spawnedWalkers; k++)
                    {
                        for (int i = 0; i < MAX_CREATE_ROOM_WALKER_ITERATIONS; i++)
                        {
                            bool createHorizontal = Random(0, 100) <= 60;
                            bool creatLowerBound = Random(0, 100) <= 40;

                            int xWalker = 0;
                            int yWalker = 0;

                            if ( createHorizontal )
                            {
                                xWalker = Random(roomX + 1, roomX + roomWidth - 1); // We don't create walkers on the corners of the room
                                yWalker = creatLowerBound ? roomY : (roomY + roomHeight);
                                if ( map->IsDoor(xWalker - 1, yWalker) == true || map->IsDoor(xWalker + 1, yWalker) == true ) // We can't have adjacent doors
                                    continue;
                            }
                            else
                            {
                                xWalker = creatLowerBound ? roomX : (roomX + roomWidth);
                                yWalker = Random(roomY + 1, roomY + roomHeight - 1); // We don't create walkers on the corners of the room
                                if ( map->IsDoor(xWalker, yWalker - 1) == true || map->IsDoor(xWalker, yWalker + 1) == true ) // We can't have adjacent doors
                                    continue;
                            }

                            if ( map->GetTile(xWalker, yWalker) != Wall ) // We can only put doors (from which the walker starts) in a wall.
                                continue;

                            // Reaching this point means we have a valid walker and door.
                            Direction walkerDirection = GetFreeDirection(xWalker, yWalker);
                            if ( walkerDirection != Direction_None )
                            {
                                map->SetTile(xWalker, yWalker, DoorWalker);
                                walkers.push_back(WalkerInstance(xWalker, yWalker, walkerDirection));
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    // Fill the whole map with walls
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if ( map->GetTile(x, y) == Corridor || map->GetTile(x, y) == Door || map->GetTile(x, y) == DoorWalker )
            {
                if ( map->IsTile(x + 1, y, Empty)  )
                    map->SetTile(x + 1, y, Wall);
                if ( map->IsTile(x + 1, y + 1, Empty) )
                    map->SetTile(x + 1, y + 1, Wall);
                if ( map->IsTile(x + 1, y - 1, Empty) )
                    map->SetTile(x + 1, y - 1, Wall);
                if ( map->IsTile(x - 1, y, Empty) )
                    map->SetTile(x - 1, y, Wall);
                if ( map->IsTile(x - 1, y + 1, Empty) )
                    map->SetTile(x - 1, y + 1, Wall);
                if ( map->IsTile(x - 1, y - 1, Empty) )
                    map->SetTile(x - 1, y - 1, Wall);
                if ( map->IsTile(x, y + 1, Empty) )
                    map->SetTile(x, y + 1, Wall);
                if ( map->IsTile(x, y - 1, Empty) )
                    map->SetTile(x, y - 1, Wall);
            }
        }
    }

    // This loop is needed to transform some marker tiles and make the map look prettier by removing unneeded walls.
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if ( map->GetTile(x, y) == DoorWalker )
            {
                int walkableConnection = 0;
                if ( map->IsTileNot(x + 1, y, Wall) ) { walkableConnection++; }
                if ( map->IsTileNot(x - 1, y, Wall) ) { walkableConnection++; }
                if ( map->IsTileNot(x, y + 1, Wall) ) { walkableConnection++; }
                if ( map->IsTileNot(x, y - 1, Wall) ) { walkableConnection++; }
                if ( walkableConnection <= 1 )
                    map->SetTile(x, y, Wall);
                else
                    map->SetTile(x, y, Door);
            }

            if ( map->GetTile(x, y) == Wall ) // If a wall has only walls and empty as adjacent, then we should delete it.
            {
                bool isWallUseless = true;
                isWallUseless &= map->IsTile(x + 1, y, Empty) || map->IsTile(x + 1, y, Wall);
                isWallUseless &= map->IsTile(x - 1, y, Empty) || map->IsTile(x - 1, y, Wall);
                isWallUseless &= map->IsTile(x, y + 1, Empty) || map->IsTile(x, y + 1, Wall);
                isWallUseless &= map->IsTile(x, y - 1, Empty) || map->IsTile(x, y - 1, Wall);
                isWallUseless &= map->IsTile(x - 1, y + 1, Empty) || map->IsTile(x - 1, y + 1, Wall);
                isWallUseless &= map->IsTile(x - 1, y - 1, Empty) || map->IsTile(x - 1, y - 1, Wall);
                isWallUseless &= map->IsTile(x + 1, y + 1, Empty) || map->IsTile(x + 1, y + 1, Wall);
                isWallUseless &= map->IsTile(x + 1, y - 1, Empty) || map->IsTile(x + 1, y - 1, Wall);
                if ( isWallUseless )
                    map->SetTile(x, y, Empty);
            }

            if ( map->GetTile(x, y) == Empty )
            {
                // If you're surrounded by walls then you should be a wall.
                if ( map->IsWall(x + 1, y) && map->IsWall(x - 1, y) ) { map->SetTile(x, y, Wall); }
                if ( map->IsWall(x, y + 1) && map->IsWall(x, y - 1) ) { map->SetTile(x, y, Wall); }
            }
        }
    }
}

bool DungeonGenerator::CreateRoom( int32_t x, int32_t y, int32_t w, int32_t h )
{
    if ( IsAreaEmpty(x, y, w, h) )
    {
        for (int i = x; i < x + w; i++)
        {
            for (int j = y; j < y + h; j++)
                map->SetTile(i, j, Floor);
        }

        for (int i = x; i < x + w; i++)
        {
            map->SetTile(i, y, Wall);
            map->SetTile(i, y + h, Wall);
        }

        for (int i = y; i < y + h; i++)
        {
            map->SetTile(x, i, Wall);
            map->SetTile(x + w, i, Wall);
        }

        map->SetTile(x + w, y + h, Wall);
        return true;
    }
    return false;
}

bool DungeonGenerator::IsAreaEmpty( int32_t x, int32_t y, int32_t w, int32_t h )
{
    if ( x + w >= map->GetWidth())
        return false;
    if ( y + h >= map->GetHeight())
        return false;
    if ( x < 0 || y < 0 )
        return false;

    for (int32_t i = x; i <= x + w; i++)
    {
        for (int32_t j = y; j <= y + h; j++)
        {
            if ( map->GetTile(i, j) != Empty )
                return false;
        }
    }
    return true;
}
