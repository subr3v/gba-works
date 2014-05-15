/********************************************************************
	created:	2014/04/09
	created:	9:4:2014   12:33
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\ShopScene.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	ShopScene
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef ShopScene_h__
#define ShopScene_h__

#include "Scene.h"
#include "GameConstants.h"
#include "stdint.h"

class TextRenderer;
class Unit;

enum ShopTab
{
    Shop_Weapon,
    Shop_Food,
    Shop_Potions,
};

struct ItemInformation
{
    ItemType Type;
    int Value;
    const char* Name;
    const char* Description;
    int CharblockID;

    ItemInformation() { }

    ItemInformation(ItemType itemType, int charblockId, int value, const char* name, const char* description)
    {
        this->Type = itemType;
        this->CharblockID = charblockId;
        this->Value = value;
        this->Name = name;
        this->Description = description;
    }
};

class GameScene;
class ShopScene : public Scene
{
    public:
        ShopScene(GameScene* gameScene);

    private:
        const char* GetAttackEffectDescription( AttackEffect effect );
        virtual void OnEnter();
        virtual void OnExit();
        virtual void Update();
        virtual void Render();

        ItemInformation GetRandomPotion();
        ItemInformation GetRandomFood();

        void GenerateItems();

        void ChangeTab(ShopTab tab);

        Unit* GetPlayer();
        ItemInformation* GetCurrentItems();

        TextRenderer* textRenderer;

        uint16_t* moneyBlock;
        uint16_t* dialogBlock;

        static const int MAX_ITEMS = 5;
        ItemInformation potionItems[MAX_ITEMS];
        ItemInformation foodItems[MAX_ITEMS];
        int selectedItem;

        ShopTab currentTab;

        int frameCounter;
        int xHandOffset;
        bool xHandMoving;

        GameScene* gameScene;
};

#endif // ShopScene_h__
