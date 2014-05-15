/********************************************************************
	created:	2014/03/12
	created:	12:3:2014   16:18
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\GraphicsUtil.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	GraphicsUtil
	file ext:	h
	author:		Stefano Musumeci

	purpose:	Contains some utility functions for graphics techniques.
*********************************************************************/
#ifndef GraphicsUtil_h__
#define GraphicsUtil_h__

#include <stdint.h>

namespace graphics
{
    void SetFadePaletteBackground(const uint16_t* palette);
    void SetFadePaletteBackgroundEntry(uint16_t index, uint16_t colour);
    void FadeOutBackground(int fadeSpeed);
    void FadeInBackground(int fadeSpeed);

    void SetFadePaletteObjects(const uint16_t* palette);
    void FadeOutObjects(int fadeSpeed);
    void FadeInObjects(int fadeSpeed);
}

#endif // GraphicsUtil_h__
