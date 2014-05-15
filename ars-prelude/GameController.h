/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   20:17
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\GameController.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	GameController
	file ext:	h
	author:		Stefano Musumeci

	purpose:	GameController provides access to different features of the game, it is the main instance
				passed between scenes and different classes.
*********************************************************************/
#ifndef GameController_h__
#define GameController_h__

struct GameWorld;
class Unit;
class Item;
class GameObject;
class GameRenderer;

#include "stdint.h"
#include <vector>
#include "Point.h"

class GameController
{
    public:
        GameController(GameRenderer* renderer, GameWorld* world);
        ~GameController();

        void MoveToNextLevel(); // Notifies the game scene that the level has been beaten
        void ClearText(); // Clears the dialogue text
        void ShowText(const char* text, ...); // Shows a new line in the dialogue text
        void EnableCameraFollowObject(bool enabled); //
        void SetCameraFollowObject(Unit* object);
        bool IsTextVisible();
        bool IsShopVisible();

        // Utility functions for gameplay logic
        bool IsCellEmpty(int x, int y, bool ignoreUnits);
        bool IsCellWalkable(int x, int y);
        bool IsTrap(int x, int y);

        // Graphics effect functions
        void DisplayUnitDamage(Unit* unit, int damage);
        void DisplayUnitLevelUp(Unit* unit);
        void ShakeCamera(int shakeIntensity = 1, int shakeDuration = 6);

        GameWorld* GetWorld();

        Unit* GetPlayer();
        GameObject* GetObject(int x, int y);
        Unit* GetUnit(int x, int y);
        Item* GetItem(int x, int y);
        std::vector<GameObject*> GetObjects(int x, int y);
        void GetRandomPosition(int& x, int& y, bool insideRoom);
        Point16 GetRandomPositionWorld(bool insideRoom);
        int GetNumberOfPlayerControlledUnits();

        Point16 GetCameraPosition();
        void OpenShop();
        void CloseShop();

        void AddObject(GameObject* object);
        void SpawnObject(GameObject* object, bool insideRoom);

        void Update();
    private:
        uint32_t dialogTimer;
        Unit* objectToFollow;
        bool cameraFollowUnit;
        Point16 cameraPosition;

        int shakeTimer;
        bool isIncreasing;
        int shakeFactor;
        int shakeIntensity;
        int shakeDuration;

        bool isShopVisible;

        GameWorld* world;
        GameRenderer* renderer;
        static const int DIALOG_TIME = 150;
};



#endif // GameController_h__
