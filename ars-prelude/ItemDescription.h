/********************************************************************
	created:	2014/04/02
	created:	2:4:2014   20:45
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\ItemDescription.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	ItemDescription
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef ItemDescription_h__
#define ItemDescription_h__

#include "GameConstants.h"
#include "stdint.h"



struct ItemDescription
{
    SpriteIdentifier spriteIdentifier;
    ItemEffect effect;
    int32_t itemParameter;
    const char* message;
    bool autoDestroy;
    bool isHumanOnly;

    ItemDescription() { }

    ItemDescription(SpriteIdentifier sprite, ItemEffect effect, const char* message, bool autoDestroy, bool isHumanOnly, int32_t parameter)
    {
        this->spriteIdentifier = sprite;
        this->effect = effect;
        this->message = message;
        this->autoDestroy = autoDestroy;
        this->itemParameter = parameter;
        this->isHumanOnly = isHumanOnly;
        this->spriteIdentifier = sprite;
    }
};


#endif // ItemDescription_h__
