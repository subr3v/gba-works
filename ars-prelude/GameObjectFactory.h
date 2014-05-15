/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   17:45
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\GameObjectFactory.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	GameObjectFactory
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef GameObjectFactory_h__
#define GameObjectFactory_h__

#include "GameConstants.h"
#include "UnitDescription.h"
#include "ItemDescription.h"

class GameObject;
class Unit;
class Item;
class GameController;

class GameObjectFactory
{
    public:
        static Unit* CreateUnit(GameController* controller, UnitType type);
        static Item* CreateItem(GameController* controller, ItemType type);
        static const UnitDescription& GetUnitDescription(UnitType type);
    private:
        static void InitializeUnitFactory();
        static void InitializeItemFactory();

        static bool isInitialized;
        static UnitDescription unitsDescription[UnitType_Count];
        static ItemDescription itemsDescription[ItemType_Count];
};


#endif // GameObjectFactory_h__
