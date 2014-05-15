/********************************************************************
	created:	2014/03/23
	created:	23:3:2014   13:16
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\FontDescription.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	FontDescription
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef FontDescription_h__
#define FontDescription_h__

#include "stdint.h"
#include <stddef.h>

struct GlyphDescription
{
    const uint16_t* glyphData;
    uint8_t width;
    uint8_t height;
    uint8_t yOffset;
    uint8_t xAdvance;

    GlyphDescription(const uint16_t* glyphData, uint8_t width, uint8_t height, uint8_t xAdvance, uint8_t yOffset)
    {
        this->glyphData = glyphData;
        this->width = width;
        this->height = height;
        this->xAdvance = xAdvance;
        this->yOffset = yOffset;
    }

    GlyphDescription()
    {
        glyphData = NULL;
        width = 0;
        height = 0;
        xAdvance = 0;
        yOffset = 0;
    }
};

struct FontDescription
{
    GlyphDescription glyphs[128];
};


#endif // FontDescription_h__
