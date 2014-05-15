/********************************************************************
	created:	2014/03/13
	created:	13:3:2014   18:51
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\ObjectGraphics.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	ObjectGraphics
	file ext:	h
	author:		Stefano Musumeci

	purpose:	Header file containing a mapping of unit sprite identifier and their sprite sheets
*********************************************************************/
#ifndef UnitGraphics_h__
#define UnitGraphics_h__

#include "GameConstants.h"
#include <stdint.h>
#include <memory>

struct SpriteDescription
{
    const uint16_t* tiles;
    int32_t xOffset;
    int32_t yOffset;
    bool renderShadow;

    SpriteDescription(const uint16_t* tiles, int32_t xOffset, int32_t yOffset, bool renderShadow)
    {
        this->tiles = tiles;
        this->xOffset = xOffset;
        this->yOffset = yOffset;
        this->renderShadow = renderShadow;
    }

    SpriteDescription() { this->tiles = 0; }
};

struct AnimationDescription
{
    const uint32_t* frames;
    int frameCount;
    int frameTime;
    bool supportsDirection;
    bool isLooped;

    AnimationDescription() { frames = 0; frameCount = 0; }
    AnimationDescription(const uint32_t* frames, int frameCount, int frameTime, bool supportsDirection, bool isLooped)
    {
        this->frames = frames;
        this->frameCount = frameCount;
        this->frameTime = frameTime;
        this->supportsDirection = supportsDirection;
        this->isLooped = isLooped;
    }
};

extern SpriteDescription objectSprites[SpriteIdentifier_Count];
extern AnimationDescription spriteAnimationFrames[Animation_Count];
void LoadUnitGraphics();
void CopySpriteFrame(uint16_t spriteIdentifier, int frameNumber, int rowNumber, uint16_t targetBlock);

#endif // UnitGraphics_h__
