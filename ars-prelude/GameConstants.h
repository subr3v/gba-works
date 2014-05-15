/********************************************************************
	created:	2014/03/12
	created:	12:3:2014   18:08
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\GameConstants.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	GameConstants
	file ext:	h
	author:		Stefano Musumeci

	purpose:	Header file containing all the constants for the game.
*********************************************************************/
#ifndef GameConstants_h__
#define GameConstants_h__

#include <stdint.h>
#include "System.h"

static const int TILE_SIZE = 24;
static const int MAX_OBJECTS = 24;

static const int MAX_MAP_SIZE = 96;

enum Direction
{
    Direction_None = 0,
    West = 1 << 0,
    East = 1 << 1,
    North = 1 << 2,
    South = 1 << 3,
};

enum SpriteIdentifier
{
    Sprite_Knight,
    Sprite_Mage,
    Sprite_Thief,
    Sprite_Slime,
    Sprite_Slime_Green,
    Sprite_Slime_Yellow,
    Sprite_Fire_Wisp,
    Sprite_Ice_Wisp,
    Sprite_Brown_Wisp,
    Sprite_Destruction_Wisp,
    Sprite_Wererat,
    Sprite_Wererat_Blue,
    Sprite_Wererat_Red,
    Sprite_Skeleton_Mage,
    Sprite_Skeleton_Thief,
    Sprite_Skeleton_Warrior,
    Sprite_Shadow,
    Sprite_Wyvern,
    Sprite_Food_Slime,
    Sprite_Food_Fire_Wisp,
    Sprite_Food_Skeleton,
    Sprite_Food_Wererat,
    Sprite_Hp_Potion_Small,
    Sprite_Hp_Potion_Medium,
    Sprite_Hp_Potion_Big,
    Sprite_Mp_Potion_Small,
    Sprite_Mp_Potion_Medium,
    Sprite_Mp_Potion_Big,
    Sprite_Potion_Yellow,
    Sprite_Potion_Orange,
    Sprite_Potion_Candy,
    Sprite_Potion_Pink,
    Sprite_Gold,
    Sprite_Spikes,
    Sprite_Trap,
    Sprite_Stairs,
    SpriteIdentifier_Count,
};

enum SpriteAnimation
{
    Still,
    StillUnit,
    Hurt,
    Walk,
    Idle,
    Attack,
    Special,
    Sleep,
    Injured,
    TrapTrigger,
    Animation_Count,
};

enum DungeonSetting
{
    Castle,
    Undersea,
    Volcano,
};

enum GameObjectType
{
    Object_None = 0,
    Object_Unit = 1,
    Object_Item = 2,
};

enum UnitType
{
    UnitType_Mage,
    UnitType_Knight,
    UnitType_Thief,
    UnitType_Morpher,
    UnitType_Blue_Slime,
    UnitType_Yellow_Slime,
    UnitType_Green_Slime,
    UnitType_Wererat,
    UnitType_Blue_Wererat,
    UnitType_Red_Wererat,
    UnitType_Fire_Wisp,
    UnitType_Ice_Wisp,
    UnitType_Destruction_Wisp,
    UnitType_Brown_Wisp,
    UnitType_Skeleton_Mage,
    UnitType_Skeleton_Warrior,
    UnitType_Skeleton_Thief,
    UnitType_Shadow,
    UnitType_Wyvern,
    UnitType_Count,
};

enum ItemType
{
    ItemType_Small_Hp_Potion,
    ItemType_Medium_Hp_Potion,
    ItemType_Big_Hp_Potion,
    ItemType_Small_Mp_Potion,
    ItemType_Medium_Mp_Potion,
    ItemType_Big_Mp_Potion,
    ItemType_Poison_Potion,
    ItemType_Sleep_Potion,
    ItemType_Teleport_Potion,
    ItemType_MysteriousPotion,
    ItemType_GrowPotion,
    ItemType_AttackEffectPotion,
    ItemType_ExperiencePotion,
    ItemType_Food_Slime,
    ItemType_Food_Slime_Exp,
    ItemType_Food_Wisp,
    ItemType_Food_Skeleton,
    ItemType_Food_Rat,
    ItemType_Food_Rat_Mystery,
    ItemType_Gold_Small,
    ItemType_Gold_Medium,
    ItemType_Gold_Big,
    ItemType_Gold_VeryBig,
    ItemType_Trap_Spikes,
    ItemType_Trap_Immobilize,
    ItemType_Trap_Sleep,
    ItemType_Trap_Poison,
    ItemType_Trap_Teleport,
    ItemType_Stairs,
    ItemType_Count,
};

enum ShopItem
{
    Shop_Item_Small_Hp,
    Shop_Item_Small_Mp,
    Shop_Item_Medium_Hp,
    Shop_Item_Medium_Mp,
    Shop_Item_Big_Hp,
    Shop_Item_Big_Mp,
    Shop_Item_Grow_Potion,
    Shop_Item_Experience_Potion,
    Shop_Item_Mysterious_Potion,
    Shop_Item_Teleport_Potion,
    Shop_Item_Attack_Enhancer_Potion,
    Shop_Item_Slime_Food,
    Shop_Item_Rat_Food,
    Shop_Item_Elemental_Food,
    Shop_Item_Skeleton_Food,
    Shop_Item_Exp_Food,
    Shop_Item_Strange_Food,
    Shop_Item_Count,
};

enum AttackEffect
{
    Attack_Effect_None,
    Attack_Effect_Poison,
    Attack_Effect_Drain_Hp,
    Attack_Effect_Drain_Mp,
    Attack_Effect_Random_Damage,
    Attack_Effect_Teleport,
    Attack_Effect_Sleep,
    Attack_Effect_Immobilize,
    Attack_Effect_Count,
};

enum ItemEffect
{
    Effect_Next_Level,
    Effect_Gold,
    Effect_Food,
    Effect_Heal_Mp,
    Effect_Heal_Hp,
    Effect_Damage_Hp,
    Effect_Damage_Mp,
    Effect_Teleport,
    Effect_Sleep,
    Effect_Immobilize,
    Effect_Poison,
    Effect_GiveExperience,
    Effect_GiveAttackEffect,
    Effect_Upgrade,
    Effect_Random,
};

static inline int32_t GetHorizontalComponent(Direction direction)
{
    if ( direction & East )
        return 1;
    else if ( direction & West )
        return -1;
    return 0;
}

static inline int32_t GetVerticalComponent(Direction direction)
{
    if ( direction & North )
        return -1;
    else if ( direction & South )
        return 1;
    return 0;
}

static inline Direction GetRandomDirection()
{
    int res = Random(0, 4);
    switch (res)
    {
        case 0:
            return East;
        case 1:
            return West;
        case 2:
            return North;
        case 3:
            return South;
        default:
            return North;
    }
}

#endif // GameConstants_h__
