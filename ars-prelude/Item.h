/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   18:48
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\Item.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	Item
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef Item_h__
#define Item_h__

#include "GameObject.h"
#include "Unit.h"
#include "GameConstants.h"
#include "ItemDescription.h"

class Item : public GameObject
{
    public:
        Item(GameController* gameController, ItemType type, const ItemDescription& itemDescription);
        void OnStep(Unit* steppedUnit);
        ItemType GetItemType();

        bool IsTrap();

        bool IsShowForeground();
        void EnableShowForeground(bool enable);
    private:
        bool showForeground;
        ItemType itemType;
        const ItemDescription& description;
};

#endif // Item_h__

