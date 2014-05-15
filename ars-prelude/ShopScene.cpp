#include "ShopScene.h"

#include "System.h"

#include "shop_menu_bg_sheet.h"
#include "shop_menu_obj_sheet.h"

#include "gba.h"
#include "GraphicsUtil.h"
#include "SceneManager.h"
#include "GameScene.h"

#include "GameController.h"
#include "Unit.h"

#include "GameFonts.h"

#include <cstdio>

enum SpriteId
{
    Hand = 0,
    SmallHp = 2,
    BigMp = 4,
    MediumMp = 6,
    SmallMp = 8,
    SlimeFood = 10,
    RatFood = 12,
    BigHp = 14,
    Knife = 32,
    Rod = 34,
    Sword = 36,
    FireFood = 38,
    MediumHp = 40,
    SkeletonFood = 42,
    YellowPotion = 44,
    OrangePotion = 46,
    HandError = 64,
    PinkPotion = 66,
    CandyPotion = 68,
};

static const char* weaponNames[21] =
{
    "Charcoal Sword",
    "Copper Sword",
    "Iron Sword",
    "Alloy Sword",
    "Bronze Sword",
    "Silver Sword",
    "Atma Sword",
    "Golden Sword",
    "Diamond Sword",
    "Ruby Sword",
    "Turquoise Sword",
    "Krypton Sword",
    "Malachite Sword",
    "Amethyst Sword",
    "Citrine Sword",
    "Agate Sword",
    "Onyx Sword",
    "Aquamarine Sword",
    "Garnet Sword",
    "Adamantium Sword",
    "Perfect Sword",
};

ItemInformation GetItem(ShopItem shopItem)
{
    static bool itemsInitialized = false;
    static ItemInformation itemsDefinition[Shop_Item_Count];
    if ( itemsInitialized == false )
    {
        itemsDefinition[Shop_Item_Small_Hp] = ItemInformation(ItemType_Small_Hp_Potion, SmallHp, 50, "Small Hp Potion", "Restores some health");
        itemsDefinition[Shop_Item_Small_Mp] = ItemInformation(ItemType_Small_Mp_Potion, SmallMp, 50, "Small Mp Potion", "Restores some magic");
        itemsDefinition[Shop_Item_Medium_Hp] = ItemInformation(ItemType_Medium_Hp_Potion, MediumHp, 150, "Medium Hp Potion", "Restores health");
        itemsDefinition[Shop_Item_Medium_Mp] = ItemInformation(ItemType_Medium_Mp_Potion, MediumMp, 150, "Medium Mp Potion", "Restores magic");
        itemsDefinition[Shop_Item_Big_Hp] = ItemInformation(ItemType_Big_Hp_Potion, BigHp, 250, "Big Hp Potion", "Restores a lot of health");
        itemsDefinition[Shop_Item_Big_Mp] = ItemInformation(ItemType_Big_Mp_Potion, BigMp, 250, "Big Mp Potion", "Restores a lot of magic");
        itemsDefinition[Shop_Item_Grow_Potion] = ItemInformation(ItemType_GrowPotion, YellowPotion, 500, "Grow Potion", "Improves your stats");
        itemsDefinition[Shop_Item_Experience_Potion] = ItemInformation(ItemType_ExperiencePotion, OrangePotion, 500, "Exp Boost Potion", "Gives you some bonus experience");
        itemsDefinition[Shop_Item_Mysterious_Potion] = ItemInformation(ItemType_MysteriousPotion, CandyPotion, 100, "Random Potion", "Random effect awaits...");
        itemsDefinition[Shop_Item_Teleport_Potion] = ItemInformation(ItemType_Teleport_Potion, CandyPotion, 125, "Teleport Potion", "Teleports you somewhere in the dungeon");
        itemsDefinition[Shop_Item_Attack_Enhancer_Potion] = ItemInformation(ItemType_AttackEffectPotion, PinkPotion, 1000, "Enhancer Potion", "Changes your attack special effect!");
        itemsDefinition[Shop_Item_Slime_Food] = ItemInformation(ItemType_Food_Slime, SlimeFood, 25, "Slime Pudding", "Tasty");
        itemsDefinition[Shop_Item_Rat_Food] = ItemInformation(ItemType_Food_Rat, RatFood, 50, "Rat Meat", "Quite Tasty");
        itemsDefinition[Shop_Item_Elemental_Food] = ItemInformation(ItemType_Food_Wisp, FireFood, 125, "Elemental Spice", "Very Tasty");
        itemsDefinition[Shop_Item_Skeleton_Food] = ItemInformation(ItemType_Food_Skeleton, SkeletonFood, 100, "Skeleton Bones", "Mysterious Taste...");
        itemsDefinition[Shop_Item_Exp_Food] = ItemInformation(ItemType_Food_Slime_Exp, SlimeFood, 250, "Tasty Slime Meat", "Experience a new taste!");
        itemsDefinition[Shop_Item_Strange_Food] = ItemInformation(ItemType_Food_Rat_Mystery, RatFood, 75, "Strange Rat Food", "Maybe you should try this");
        itemsInitialized = true;
    }

    return itemsDefinition[shopItem];
}

const uint16_t weaponShopBlock[30 * 20] =
{
    88, 89, 19, 209, 210, 211, 212, 213, 214, 19, 19, 19, 119, 120, 121, 122, 123, 124, 19, 19, 19, 161, 162, 163, 164, 165, 166, 19, 86, 87,
    104, 105, 19, 225, 226, 227, 228, 229, 230, 19, 19, 19, 135, 136, 137, 138, 139, 140, 19, 19, 19, 177, 178, 179, 180, 181, 182, 19, 102, 103,
    19, 19, 19, 241, 242, 243, 244, 245, 246, 19, 19, 19, 151, 152, 153, 154, 155, 156, 19, 19, 19, 193, 194, 195, 196, 197, 198, 19, 19, 19,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4,
    17, 13, 14, 15, 16, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 167, 168, 169, 170, 171, 172, 173, 174, 20,
    17, 29, 30, 31, 32, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 183, 184, 185, 186, 187, 188, 189, 190, 20,
    17, 45, 46, 47, 48, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 61, 62, 63, 64, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 77, 78, 79, 80, 18, 18, 18, 18, 141, 142, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 90, 91, 92, 18, 18, 18, 18, 93, 94, 95, 96, 18, 18, 18, 18, 157, 158, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 109, 110, 111, 112, 18, 18, 18, 18, 143, 144, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 106, 107, 108, 18, 18, 18, 18, 125, 126, 127, 128, 18, 18, 18, 18, 159, 160, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 237, 238, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 38, 39, 40, 41, 42, 43, 44,
    17, 18, 253, 254, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 54, 55, 56, 57, 58, 59, 60,
    25, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 70, 71, 72, 73, 74, 75, 76,
    19, 33, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 36, 37, 19,
    19, 49, 50, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 52, 53, 19,
    19, 65, 66, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 68, 69, 19,
    19, 81, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 84, 85, 19,
    19, 97, 98, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 100, 101, 19
};

const uint16_t potionShopBlock[30 * 20] =
{
    88, 89, 19, 113, 114, 115, 116, 117, 118, 19, 19, 19, 119, 120, 121, 122, 123, 124, 19, 19, 19, 257, 258, 259, 260, 261, 262, 19, 86, 87,
    104, 105, 19, 129, 130, 131, 132, 133, 134, 19, 19, 19, 135, 136, 137, 138, 139, 140, 19, 19, 19, 273, 274, 275, 276, 277, 278, 19, 102, 103,
    19, 19, 19, 145, 146, 147, 148, 149, 150, 19, 19, 19, 151, 152, 153, 154, 155, 156, 19, 19, 19, 289, 290, 291, 292, 293, 294, 19, 19, 19,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 141, 142, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 157, 158, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 143, 144, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 159, 160, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 38, 39, 40, 41, 42, 43, 44,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 54, 55, 56, 57, 58, 59, 60,
    25, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 70, 71, 72, 73, 74, 75, 76,
    19, 33, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 36, 37, 19,
    19, 49, 50, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 52, 53, 19,
    19, 65, 66, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 68, 69, 19,
    19, 81, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 84, 85, 19,
    19, 97, 98, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 100, 101, 19
};

const uint16_t foodShopBlock[30 * 20] =
{
    88, 89, 19, 113, 114, 115, 116, 117, 118, 19, 19, 19, 215, 216, 217, 218, 219, 220, 19, 19, 19, 161, 162, 163, 164, 165, 166, 19, 86, 87,
    104, 105, 19, 129, 130, 131, 132, 133, 134, 19, 19, 19, 231, 232, 233, 234, 235, 236, 19, 19, 19, 177, 178, 179, 180, 181, 182, 19, 102, 103,
    19, 19, 19, 145, 146, 147, 148, 149, 150, 19, 19, 19, 247, 248, 249, 250, 251, 252, 19, 19, 19, 193, 194, 195, 196, 197, 198, 19, 19, 19,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 18, 205, 206, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 221, 222, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 38, 39, 40, 41, 42, 43, 44,
    17, 18, 18, 18, 18, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 18, 199, 200, 200, 201, 18, 54, 55, 56, 57, 58, 59, 60,
    25, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 70, 71, 72, 73, 74, 75, 76,
    19, 33, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 36, 37, 19,
    19, 49, 50, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 52, 53, 19,
    19, 65, 66, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 68, 69, 19,
    19, 81, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 84, 85, 19,
    19, 97, 98, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 100, 101, 19
};

ShopScene::ShopScene( GameScene* gameScene )
{
    this->gameScene = gameScene;
}

void ShopScene::OnEnter()
{
    hardware::SetupDisplayControl(false, false, false, false, false);
    ClearObjects();

    for (int i = 0; i < 8192; i++)
    {
        hardware::Charblocks[0].data[i] = shop_menu_bg_sheetTiles[i];
        hardware::Charblocks[1].data[i] = shop_menu_bg_sheetTiles[i + 8192];
        hardware::Charblocks[4].data[i] = shop_menu_obj_sheetTiles[i];
    }

    graphics::SetFadePaletteBackground(shop_menu_bg_sheetPal);
    graphics::SetFadePaletteObjects(shop_menu_obj_sheetPal);

    textRenderer = new TextRenderer(1, 2, 30, 0);

    selectedItem = 0;
    xHandOffset = 0;
    xHandMoving = true;
    frameCounter = 0;

    GenerateItems();
    ChangeTab(Shop_Potions);

    Render();
    hardware::SetupDisplayControl(true, true, true, false, false);
    hardware::SetupBackground(0, 0, 31, false, 1);

    graphics::FadeInBackground(0);
    graphics::FadeInObjects(0);
}

void ShopScene::GenerateItems()
{
    for (int i = 0; i < 5; i++)
    {
        potionItems[i] = GetRandomPotion();
        foodItems[i] = GetRandomFood();
    }
}

void ShopScene::OnExit()
{
    delete textRenderer;

    graphics::FadeOutObjects(0);
    graphics::FadeOutBackground(0);
}

void ShopScene::Update()
{
    if ( gameScene->GetGameController()->IsShopVisible() == false )
    {
        GetSceneManager()->SetScene(gameScene, true);
        return;
    }

    if ( hardware::IsButtonPressed(KEY_R) )
    {
        if ( currentTab == Shop_Weapon )
        {
            ChangeTab(Shop_Food);
        }
        else if ( currentTab == Shop_Food )
        {
            ChangeTab(Shop_Potions);
        }
    }
    if ( hardware::IsButtonPressed(KEY_L))
    {
        if ( currentTab == Shop_Potions )
        {
            ChangeTab(Shop_Food);
        }
        else if ( currentTab == Shop_Food )
        {
            ChangeTab(Shop_Weapon);
        }
    }

    if ( hardware::IsButtonPressed(KEY_B))
    {
        gameScene->GetGameController()->CloseShop();
    }

    if ( currentTab == Shop_Food || currentTab == Shop_Potions )
    {
        int previousSelectedItem = selectedItem;

        ItemInformation* items = GetCurrentItems();

        if ( selectedItem + 1 < MAX_ITEMS && hardware::IsButtonPressed(KEY_DOWN))
        {
            selectedItem++;
        }
        if ( selectedItem > 0 && hardware::IsButtonPressed(KEY_UP))
        {
            selectedItem--;
        }

        if ( previousSelectedItem != selectedItem )
        {
            textRenderer->ClearTextArea(dialogBlock, 5 * 24);
            textRenderer->RenderString<26>(normalFont, dialogBlock, items[selectedItem].Description, strlen(items[selectedItem].Description), 6, 254, 253);
        }

        if ( hardware::IsButtonPressed(KEY_A))
        {
            if ( items[selectedItem].Value <= GetPlayer()->GetGold() )
            {
                GetPlayer()->ModifyGold(-items[selectedItem].Value);
                GetPlayer()->UseItem(items[selectedItem].Type);
                gameScene->GetGameController()->CloseShop();
            }
        }

        if ( hardware::IsButtonPressed(KEY_SELECT) )
        {
            GenerateItems();
            ChangeTab(currentTab);
        }
    }
    else
    {
        Unit* player = GetPlayer();
        if ( hardware::IsButtonPressed(KEY_A) && player->GetGold() >= player->GetUpgradeCost() && player->GetNumberOfUpgrades() <= 99)
        {
            player->ModifyGold(-player->GetUpgradeCost());
            player->UpgradeRandom();
            ChangeTab(Shop_Weapon); // This will refresh all the stats.
        }
        if ( hardware::IsButtonPressed(KEY_SELECT))
        {
            player->GiveExperience(9999);
            ChangeTab(Shop_Weapon); // This will refresh all the stats.
        }
    }
}

void ShopScene::Render()
{
    frameCounter++;
    if ( frameCounter % 6 == 0 )
    {
        if ( xHandMoving )
            xHandOffset ++;
        else
            xHandOffset --;

        if ( xHandOffset == 4 )
            xHandMoving = false;

        if ( xHandMoving == false && xHandOffset == 0 )
            xHandMoving = true;

        frameCounter = 0;
    }

    GameController* gameController = gameScene->GetGameController();

    Unit* player = gameController->GetPlayer();

    textRenderer->ClearTextArea(moneyBlock, 6);
    char moneyText[6];
    sprintf(moneyText, "%d", player->GetGold());
    textRenderer->RenderString<4>(outlineFont, moneyBlock, moneyText, strlen(moneyText), 0, 253, 254);

    // Render current cursor

    if ( currentTab == Shop_Food || currentTab == Shop_Potions )
    {
        ItemInformation* items = GetCurrentItems();
        SpriteId handId = Hand;
        if ( items[selectedItem].Value > player->GetGold() )
            handId = HandError;
        hardware::SetSprite8(0, 4 + xHandOffset, 32 + selectedItem * 16, handId, 0, 1);
    }
    else
    {
        hardware::HideSprite(0);
    }

    UpdateObjects();
}

void ShopScene::ChangeTab( ShopTab tab )
{
    currentTab = tab;
    textRenderer->ResetAreas();
    ClearObjects();

    const uint16_t* tabBlock = NULL;
    switch (tab)
    {
        case Shop_Weapon:
            tabBlock = weaponShopBlock;
            break;
        case Shop_Food:
            tabBlock = foodShopBlock;
            break;
        case Shop_Potions:
            tabBlock = potionShopBlock;
            break;
    }

    moneyBlock = textRenderer->CreateTextArea(25, 13, 6, 1);
    dialogBlock = textRenderer->CreateTextArea(3, 15, 26, 5);

    hardware::Screenblock* screenblock = &hardware::Screenblocks[31];
    for (int x = 0; x < 30; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            screenblock->SetTile(x, y, tabBlock[y * 30 + x] - 1);
        }
    }

    if ( tab == Shop_Weapon )
    {
        uint16_t* weaponNameBlock = textRenderer->CreateTextArea(5, 5, 15, 1);
        uint16_t* hpBlock = textRenderer->CreateTextArea(20, 9, 4, 1);
        uint16_t* mpBlock = textRenderer->CreateTextArea(20, 11, 4, 1);
        uint16_t* atkBlock = textRenderer->CreateTextArea(5, 9, 4, 1);
        uint16_t* defBlock = textRenderer->CreateTextArea(5, 11, 4, 1);
        uint16_t* matkBlock = textRenderer->CreateTextArea(13, 9, 4, 1);
        uint16_t* mdefBlock = textRenderer->CreateTextArea(13, 11, 4, 1);
        uint16_t* upgradeCostBlock = textRenderer->CreateTextArea(23, 6, 5, 1);
        uint16_t* levelBlock = textRenderer->CreateTextArea(5, 13, 3, 1);

        hardware::SetSprite8(1, 16, 40, Sword, 0, 1);

        Unit* player = GetPlayer();
        int upgradeCost = player->GetUpgradeCost();

        char costText[6]; sprintf(costText, "%d", upgradeCost);
        char atkText[6]; sprintf(atkText, "%d", player->GetAttack());
        char matkText[6]; sprintf(matkText, "%d", player->GetMagicAttack());
        char defText[6]; sprintf(defText, "%d", player->GetDefense());
        char mdefText[6]; sprintf(mdefText, "%d", player->GetMagicDefense());
        char healthText[6]; sprintf(healthText, "%d", player->GetMaxHealth());
        char magicText[6]; sprintf(magicText, "%d", player->GetMaxMagic());

        int numberOfUpgrades = player->GetNumberOfUpgrades();
        int upgradeStage = numberOfUpgrades / 5;
        int upgradeCount = (numberOfUpgrades % 5);

        char weaponName[30];
        sprintf(weaponName, "%s + %d", weaponNames[upgradeStage], upgradeCount);

        char lvlText[3];
        sprintf(lvlText, "%d", player->GetLevel());

        textRenderer->RenderString<12>(outlineFont, weaponNameBlock, weaponName, strlen(weaponName), 0, 253, 254);
        textRenderer->RenderString<5>(outlineFont, upgradeCostBlock, costText, strlen(costText), 0, 253, 254);
        textRenderer->RenderString<4>(outlineFont, atkBlock, atkText, strlen(atkText), 0, 253, 254);
        textRenderer->RenderString<4>(outlineFont, defBlock, defText, strlen(defText), 0, 253, 254);

        textRenderer->RenderString<4>(outlineFont, hpBlock, healthText, strlen(healthText), 0, 253, 254);
        textRenderer->RenderString<4>(outlineFont, mpBlock, magicText, strlen(magicText), 0, 253, 254);

        textRenderer->RenderString<4>(outlineFont, matkBlock, matkText, strlen(matkText), 0, 253, 254);
        textRenderer->RenderString<4>(outlineFont, mdefBlock, mdefText, strlen(mdefText), 0, 253, 254);

        textRenderer->RenderString<3>(outlineFont, levelBlock, lvlText, strlen(lvlText), 0, 253, 254);

        const char* weaponShopDescription;
        if ( numberOfUpgrades == 100 )
            weaponShopDescription = "You reached the maximum upgrade";
        else if ( player->GetGold() >= upgradeCost )
            weaponShopDescription = "Press A to receive a random upgrade";
        else
            weaponShopDescription = "Not enough money, come back later";

        const char* effectText = "Effect:";
        const char* weaponEffectDescription = GetAttackEffectDescription(player->GetAttackEffect());

        textRenderer->RenderString<26>(normalFont, dialogBlock, weaponShopDescription, strlen(weaponShopDescription), 6, 254, 253);
        textRenderer->RenderString<26>(normalFont, dialogBlock, effectText, strlen(effectText), 2 + 6 + 8, 254, 253);
        textRenderer->RenderString<26>(normalFont, dialogBlock, weaponEffectDescription, strlen(weaponEffectDescription), 4 + 6 + 16, 254, 253);
    }
    else // Food or Potion.
    {
        uint16_t* nameBlocks[5];
        uint16_t* priceBlocks[5];

        for (int i = 0; i < 5; i++)
        {
            nameBlocks[i]  = textRenderer->CreateTextArea(5 , 4 + i * 2, 12, 1);
            priceBlocks[i] = textRenderer->CreateTextArea(18, 4 + i * 2, 5, 1);
        }

        ItemInformation* items = GetCurrentItems();

        for (int i = 0; i < 5; i++)
        {
            char priceText[5];
            sprintf(priceText, "%d", items[i].Value);
            textRenderer->RenderString<12>(outlineFont, nameBlocks[i], items[i].Name, strlen(items[i].Name), 0, 253, 254);
            textRenderer->RenderString<5>(outlineFont, priceBlocks[i], priceText, strlen(priceText), 0, 253, 254);
            hardware::SetSprite8(i + 1, 20, 28 + i * 16, items[i].CharblockID, 0, 1);
        }

        textRenderer->RenderString<26>(normalFont, dialogBlock, items[selectedItem].Description, strlen(items[selectedItem].Description), 6, 254, 253);

        if ( tab == Shop_Potions)
        {
            uint16_t* hpBlock = textRenderer->CreateTextArea(23, 6, 7, 2);
            uint16_t* mpBlock = textRenderer->CreateTextArea(23, 9, 7, 2);

            Unit* player = GetPlayer();
            uint16_t  playerHp = player->GetHealth();
            uint16_t  playerMp = player->GetMagic();
            uint16_t  playerMaxHp = player->GetMaxHealth();
            uint16_t  playerMaxMp = player->GetMaxMagic();

            char hpText[7];
            sprintf(hpText, "%d/%d", playerHp, playerMaxHp);

            char mpText[7];
            sprintf(mpText, "%d/%d", playerMp, playerMaxMp);

            textRenderer->RenderString<7>(outlineFont, hpBlock, hpText, 7, 2, 253, 254);
            textRenderer->RenderString<7>(outlineFont, mpBlock, mpText, 7, 2, 253, 254);
        }
        else // Shop Food
        {
            uint16_t* foodBlock = textRenderer->CreateTextArea(26, 5, 4, 2);
            Unit* player = GetPlayer();

            uint16_t playerFood = player->GetFood();
            uint16_t playerFoodPercent = playerFood * 100 / player->GetMaxFood();
            char foodText[5];
            sprintf(foodText, "%d%%", playerFoodPercent);
            textRenderer->RenderString<4>(outlineFont, foodBlock, foodText, 5, 4, 253, 254);
        }
    }
}

ItemInformation* ShopScene::GetCurrentItems()
{
    return currentTab == Shop_Potions ? potionItems : foodItems;
}

Unit* ShopScene::GetPlayer()
{
    return gameScene->GetGameController()->GetPlayer();
}

const char* ShopScene::GetAttackEffectDescription( AttackEffect effect )
{
    switch (effect)
    {
        case Attack_Effect_None:
            return "None";
            break;
        case Attack_Effect_Poison:
            return "Poison target (15% success)";
            break;
        case Attack_Effect_Drain_Hp:
            return "Get 1/4 of damage dealt back in Hp";
            break;
        case Attack_Effect_Drain_Mp:
            return "Get 1/4 of damage dealt back in Mp";
            break;
        case Attack_Effect_Random_Damage:
            return "Damage randomised";
            break;
        case Attack_Effect_Teleport:
            return "Teleport target (15% success)";
            break;
        case Attack_Effect_Sleep:
            return "Put target asleep (15% success)";
            break;
        case Attack_Effect_Immobilize:
            return "Immobilize target (15% success)";
            break;
        default:
            return "Error";
            break;
    }
}

ItemInformation ShopScene::GetRandomPotion()
{
    return GetItem((ShopItem)Random(0, 11));
}

ItemInformation ShopScene::GetRandomFood()
{
    return GetItem((ShopItem)Random(12, 17));
}
