/********************************************************************
	created:	2014/03/29
	created:	29:3:2014   23:36
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\UnitDescription.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	UnitDescription
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef UnitDescription_h__
#define UnitDescription_h__

#include "stdint.h"
#include <cstring>
#include "GameConstants.h"

struct UnitDescription
{
    SpriteIdentifier spriteIdentifier;

    float startingHealth;
    float startingMagic;
    float physicalAttack;
    float magicalAttack;
    float physicalDefense;
    float magicalDefense;

    float healthGrowth;
    float magicGrowth;

    float physicalAttackGrowth;
    float magicalAttackGrowth;
    float physicalDefenseGrowth;
    float magicalDefenseGrowth;

    AttackEffect attackEffect;

    ItemType droppedItem;
    int16_t experienceGiven;
    const char* Name;

    UnitDescription()
    {
        startingHealth = startingMagic = 0;
        physicalAttack = physicalDefense = 0;
        magicalAttack = magicalDefense = 0;
        physicalAttackGrowth = physicalDefenseGrowth = 0;
        magicalAttackGrowth = magicalDefenseGrowth = 0;
        healthGrowth = magicGrowth = 0;
        attackEffect = Attack_Effect_None;
        droppedItem = ItemType_Count;
        experienceGiven = 0;
        Name = "None";
    }
};

#endif // UnitDescription_h__
