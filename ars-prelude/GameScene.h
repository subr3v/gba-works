/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   19:56
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\GameScene.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	GameScene
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef GameScene_h__
#define GameScene_h__

#include <vector>
#include "Scene.h"
#include "Point.h"
#include "GameConstants.h"
#include "DungeonMap.h"
#include "System.h"
#include "GameRenderer.h"
#include "DungeonGenerator.h"
#include "DungeonLevel.h"

class Unit;
class Command;
class GameController;
class GameWorld;

class GameScene : public Scene
{
    public:
        GameScene(UnitType playerType);
        ~GameScene();

        virtual void Update();
        virtual void Render();
        virtual void OnEnter();
        virtual void OnExit();

        GameController* GetGameController();

    private:

        void GenerateDungeon();

        void SpawnMonster(UnitType type);

        int enemySpawnCounter;
        static const int ENEMY_SPAWN_TIME = 3600;

        GameWorld* world;
        GameController* gameController;

        Command* currentCommand;
        uint8_t currentUnit;
        GameRenderer gameRenderer;

        DungeonGenerator* dungeonGenerator;
        DungeonMap currentMap;
        int currentDungeonLevel;
        DungeonLevel currentLevel;
};

#endif // GameScene_h__
