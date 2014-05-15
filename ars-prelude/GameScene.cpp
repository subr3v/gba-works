#include "GameScene.h"
#include "gba.h"
#include "GraphicsUtil.h"
#include "Item.h"
#include "Unit.h"
#include <cstdio>
#include <cstring>
#include "Gameworld.h"
#include "GameController.h"
#include "CharacterMenuScene.h"
#include "SceneManager.h"
#include "GameObjectFactory.h"
#include "DungeonLevel.h"
#include "Command.h"
#include "IntroScene.h"
#include "ShopScene.h"
#include "CutsceneScene.h"

GameScene::GameScene(UnitType playerType) : Scene()
{
    currentCommand = NULL;
    currentUnit = 0;
    currentDungeonLevel = -1;
    world = new GameWorld(&currentMap);
    gameController = new GameController(&gameRenderer, world);
    dungeonGenerator = new DungeonGenerator(&currentMap);
    Unit* player = GameObjectFactory::CreateUnit(gameController, playerType); // Create player from type.
    player->EnableHumanControlled(true);
    world->SetPlayer(player);
    world->isMapBeaten = true; // Start with map beaten so we generate a dungeon for the player.
}

GameScene::~GameScene()
{
    delete gameController;
    delete world;
    delete dungeonGenerator;
    if (currentCommand != NULL)
        delete currentCommand;
}

void GameScene::Update()
{
    if ( world->player->GetHealth() <= 0 )
    {
        // We're dead so..
        // Change scene.
        graphics::FadeOutBackground(1);
        graphics::FadeOutObjects(1);
        GetSceneManager()->SetScene(new CharacterMenuScene(), true);
    }

    if ( world->isMapBeaten )
    {
        graphics::FadeOutBackground(2);
        graphics::FadeOutObjects(2);
        GenerateDungeon();
    }

    if ( gameController->IsShopVisible() )
    {
        GetSceneManager()->SetScene(new ShopScene(this), false);
        return;
    }

    world->Update();
    gameController->Update();

    if ( currentCommand == NULL )
    {
        GameObject* currentObject = world->objects[currentUnit];
        if ( currentObject != NULL && currentObject->GetType() == Object_Unit )
        {
            Unit* currentUnit = (Unit*)currentObject;
            Command* command = currentUnit->ThinkCommand();
            if ( currentUnit->IsHumanControlled() )
                gameController->SetCameraFollowObject(currentUnit);
            if ( command != NULL )
                currentCommand = command;
        }
    }

    if ( currentCommand != NULL )
    {
        if ( currentCommand->Execute() == Done )
        {
            delete currentCommand;
            currentCommand = NULL;
            do
            {
                currentUnit++;
                currentUnit %= MAX_OBJECTS;
            }
            while (world->objects[currentUnit] == NULL || world->objects[currentUnit]->GetType() != Object_Unit);
        }
    }

    enemySpawnCounter++;
    if ( enemySpawnCounter > ENEMY_SPAWN_TIME )
    {

        int aliveEnemies = 0;
        for (int i = 0; i < MAX_OBJECTS; i++)
        {

            if ( world->objects[i] != NULL && world->objects[i]->GetType() == Object_Unit && world->objects[i]->AsUnit()->IsHumanControlled() == false )
                aliveEnemies++;
        }
        if ( aliveEnemies == 0 ) // If you killed everyone then the shadow comes.
        {
            SpawnMonster(UnitType_Shadow);
        }
        else if ( aliveEnemies < currentLevel.monstersCount )
        {
            SpawnMonster(UnitType_Count);
            enemySpawnCounter = 0;
        }
    }
}

void GameScene::Render()
{
    Unit* player = world->player;
    Point16 cameraPosition = gameController->GetCameraPosition();
    gameRenderer.SetCamera(cameraPosition.x, cameraPosition.y);
    gameRenderer.RenderDungeon();
    gameRenderer.RenderObjects(world->objects);
    gameRenderer.RenderInterface(gameController->IsTextVisible(), player->GetHealth(),
                                 player->GetMaxHealth(), player->GetMagic(), player->GetMaxMagic(),
                                 player->GetFood(), player->GetMaxFood(), player->GetExperience(), player->GetNeededExperience());
}

void GameScene::OnEnter()
{
    gameRenderer.Init();
    if ( currentMap.GetWidth() != 0 )
        gameRenderer.LoadMap(currentMap, currentLevel.setting);
}

void GameScene::OnExit()
{
    gameRenderer.Shutdown();
}

void GameScene::GenerateDungeon()
{
    // Reset & cleanup game information.
    if (currentCommand != NULL)
        delete currentCommand;
    currentCommand = NULL;
    currentUnit = 0;

    currentDungeonLevel++;
    if ( currentDungeonLevel >= DungeonLevel::GetNumberOfLevels() ) // Obviously you beat the game or something like that.
    {
        currentDungeonLevel = 0;
        GetSceneManager()->SetScene(new CutsceneScene(4, new IntroScene()), true);
        return;
    }

    currentLevel = DungeonLevel::GetLevel(currentDungeonLevel);
    dungeonGenerator->GenerateDungeon(currentLevel.width, currentLevel.height, currentLevel.roomNumber);
    gameRenderer.LoadMap(currentMap, currentLevel.setting);
    world->StartLevel();

    // Spawn player in a random room.
    world->player->position = gameController->GetRandomPositionWorld(true);

    // Spawn stairs in a random room if we're not in wyvern level
    if ( (currentDungeonLevel + 1) % 5 != 0)
    {
        GameObject* stairs = GameObjectFactory::CreateItem(gameController, ItemType_Stairs);
        stairs->position = gameController->GetRandomPositionWorld(true);
        world->AddObject(stairs);
    }
    else
    {
        Unit* wyvern = GameObjectFactory::CreateUnit(gameController, UnitType_Wyvern);
        wyvern->position = gameController->GetRandomPositionWorld(true);
        wyvern->LevelUpTo(currentDungeonLevel + 1);
        world->AddObject(wyvern);
    }

    for (int i = 0; i < currentLevel.treasuresCount; i++)
    {
        int goldModifier = Random(0, 3); // + 0 is small, 1 is medium , 2 is big and 3 is very big.
        ItemType goldType = (ItemType)(ItemType_Gold_Small + goldModifier);
        gameController->SpawnObject(GameObjectFactory::CreateItem(gameController, goldType), false);
    }

    // We always spawn 1 random food in the map.
    ItemType foodType = (ItemType)(ItemType_Food_Slime + Random(0, 3));
    gameController->SpawnObject(GameObjectFactory::CreateItem(gameController, foodType), true);

    for (int i = 0; i < currentLevel.trapsCount; i++)
    {
        ItemType trapType = (ItemType)(ItemType_Trap_Spikes + Random(0, 4));
        gameController->SpawnObject(GameObjectFactory::CreateItem(gameController, trapType), false);
    }

    for (int i = 0; i < currentLevel.itemsCount; i++)
    {
        ItemType itemType = (ItemType)(ItemType_Small_Hp_Potion + Random(0, 12));
        gameController->SpawnObject(GameObjectFactory::CreateItem(gameController, itemType), false);
    }

    for (int i = 0 ; i < currentLevel.monstersCount - 1; i++)
    {
        SpawnMonster(UnitType_Count); // Random monster
    }
    if ( Random(0, 100) <= 25 && currentDungeonLevel > 3 ) // 25% it's a shadow mini-boss and from level 3
    {
        SpawnMonster(UnitType_Shadow);
    }
    else
    {
        SpawnMonster(UnitType_Count);
    }


    gameController->ClearText();
    gameRenderer.ShowDungeonLevel(currentDungeonLevel + 1);
    gameController->EnableCameraFollowObject(true);

    if ( currentDungeonLevel % 5 == 0 )
    {
        GetSceneManager()->SetScene(new CutsceneScene(currentDungeonLevel / 5, this), false);
    }
    else
    {
        gameController->Update();
        this->Render();
        graphics::FadeInBackground(1);
        graphics::FadeInObjects(2);
    }
}

GameController* GameScene::GetGameController()
{
    return gameController;
}

void GameScene::SpawnMonster(UnitType type)
{
    if ( type == UnitType_Count )
        type = (UnitType)(UnitType_Blue_Slime + Random(0, 13));
    Unit* monster = GameObjectFactory::CreateUnit(gameController, type);
    monster->LevelUpTo(currentLevel.monstersLevel);
    if ( Random(0, 100) <= 30 )
        monster->InflictSleep();
    gameController->SpawnObject(monster, false);
}

