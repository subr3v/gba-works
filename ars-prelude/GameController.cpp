#include "GameController.h"
#include "Item.h"
#include "Unit.h"
#include "GameWorld.h"
#include "DungeonMap.h"
#include "GameRenderer.h"
#include "gba.h"
#include <cstdarg>
#include <cstdio>

GameController::GameController( GameRenderer* renderer, GameWorld* world )
{
    this->world = world;
    this->renderer = renderer;
    dialogTimer = 0;
    cameraFollowUnit = true;
    isShopVisible = false;
    shakeFactor = 0;
    shakeTimer = 0;
    isIncreasing = false;
    shakeDuration = 0;
    shakeIntensity = 0;
}

GameController::~GameController()
{

}

void GameController::Update()
{
    dialogTimer++;

    if ( cameraFollowUnit )
    {
        cameraPosition.x = objectToFollow->position.x - (SCREEN_WIDTH  / 2 - 16 / 2);
        cameraPosition.y = objectToFollow->position.y - (SCREEN_HEIGHT / 2 - 16 / 2);
    }

    if ( shakeTimer < shakeDuration )
    {
        shakeTimer++;
        shakeFactor += isIncreasing ? 1 : -1;
        if ( shakeFactor < 0 && shakeFactor < -shakeIntensity || shakeFactor > shakeIntensity )
        {
            isIncreasing = !isIncreasing;
        }
    }
    else
        shakeFactor = 0;
}

void GameController::GetRandomPosition( int& x, int& y, bool insideRoom )
{
    DungeonMap* map = world->map;
    while (true)
    {
        x = Random(0, world->map->GetWidth());
        y = Random(0, world->map->GetHeight());

        if ( map->IsWalkable(x, y) == false || IsCellEmpty(x, y, false) == false )
            continue;

        if ( insideRoom == false ) // We don't need to check anything else
            break;
        else
        {
            if ( map->IsFloor(x, y) == false )
                continue;
            if ( map->IsCorridor(x + 1, y) || map->IsCorridor(x - 1, y) || map->IsCorridor(x, y + 1) || map->IsCorridor(x, y - 1))
                continue;

            break;
        }
    }
}

Point16 GameController::GetRandomPositionWorld( bool insideRoom )
{
    int x, y;
    GetRandomPosition(x, y, insideRoom);
    return Point16(x * TILE_SIZE, y * TILE_SIZE);
}

GameObject* GameController::GetObject( int x, int y )
{
    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        Point16 unitPosition;
        if ( world->objects[i] != NULL)
        {
            unitPosition = world->objects[i]->GetGridPosition();
            if ( x == unitPosition.x && y == unitPosition.y )
            {
                return world->objects[i];
            }
        }
    }
    return NULL;
}

Unit* GameController::GetUnit( int x, int y )
{
    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        Point16 unitPosition;
        if ( world->objects[i] != NULL)
        {
            unitPosition = world->objects[i]->GetGridPosition();
            if ( x == unitPosition.x && y == unitPosition.y && world->objects[i]->GetType() == Object_Unit)
            {
                return (Unit*)world->objects[i];
            }
        }
    }
    return NULL;
}


Item* GameController::GetItem( int x, int y )
{
    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        Point16 unitPosition;
        if ( world->objects[i] != NULL)
        {
            unitPosition = world->objects[i]->GetGridPosition();
            if ( world->objects[i]->GetType() == Object_Item && x == unitPosition.x && y == unitPosition.y )
            {
                return (Item*)world->objects[i];
            }
        }
    }
    return NULL;
}

std::vector<GameObject*> GameController::GetObjects( int x, int y )
{
    std::vector<GameObject*> cellObjects;
    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        Point16 unitPosition;
        if ( world->objects[i] != NULL)
        {
            unitPosition = world->objects[i]->GetGridPosition();
            if ( x == unitPosition.x && y == unitPosition.y )
            {
                cellObjects.push_back(world->objects[i]);
            }
        }
    }
    return cellObjects;
}

bool GameController::IsCellWalkable( int x, int y )
{
    return world->map->IsWalkable(x, y);
}

bool GameController::IsCellEmpty( int x, int y, bool ignoreUnits )
{
    if ( world->map->IsWall(x, y) == false )
    {
        if ( ignoreUnits )
            return GetUnit(x, y) == NULL;
        else
            return GetObject(x, y) == NULL;
    }
    return false;
}

void GameController::ShowText(const char* text, ... )
{
    char lineText[31];
    va_list list;
    va_start(list, text);
    vsprintf(lineText, text, list);
    va_end(list);
    renderer->DisplayText(lineText, 30);
    dialogTimer = 0;
}

void GameController::MoveToNextLevel()
{
    world->isMapBeaten = true;
}

Unit* GameController::GetPlayer()
{
    return world->player;
}

Point16 GameController::GetCameraPosition()
{
    Point16 cameraPos = cameraPosition;
    cameraPos.x += shakeFactor;
    return cameraPos;
}

bool GameController::IsTextVisible()
{
    return dialogTimer < DIALOG_TIME;
}

void GameController::ClearText()
{
    renderer->ClearText();
}

void GameController::EnableCameraFollowObject( bool enabled )
{
    cameraFollowUnit = enabled;
}

void GameController::SetCameraFollowObject( Unit* object )
{
    objectToFollow = object;
    if ( object == NULL )
        objectToFollow = world->player;
}

void GameController::SpawnObject( GameObject* object, bool insideRoom )
{
    object->position = GetRandomPositionWorld(insideRoom);
    world->AddObject(object);
}

void GameController::AddObject( GameObject* object )
{
    world->AddObject(object);
}

bool GameController::IsTrap( int x, int y )
{
    Item* item = GetItem(x, y);
    if (item != NULL)
    {
        return item->IsTrap();
    }
    return false;
}

void GameController::OpenShop()
{
    isShopVisible = true;
}

void GameController::CloseShop()
{
    isShopVisible = false;
}

bool GameController::IsShopVisible()
{
    return isShopVisible;
}

int GameController::GetNumberOfPlayerControlledUnits()
{
    int playerUnits = 0;
    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        if ( world->objects[i] != NULL && world->objects[i]->AsUnit() != NULL && world->objects[i]->AsUnit()->IsHumanControlled() )
            playerUnits++;
    }
    return playerUnits;
}

GameWorld* GameController::GetWorld()
{
    return world;
}

void GameController::DisplayUnitDamage( Unit* unit, int damage )
{
    Point16 damagePos = unit->position;
    damagePos.y -= 12;
    damagePos.x += 4;
    renderer->DisplayDamage(damagePos, damage);
}

void GameController::DisplayUnitLevelUp( Unit* unit )
{
    Point16 textPos = unit->position;
    textPos.y -= 14;
    textPos.x += 2;
    renderer->DisplayLevelUp(textPos);
}

void GameController::ShakeCamera(int shakeIntensity, int shakeDuration)
{
    isIncreasing = false;
    shakeFactor = 0;
    shakeTimer = 0;
    this->shakeDuration = shakeDuration;
    this->shakeIntensity = shakeIntensity;
}

