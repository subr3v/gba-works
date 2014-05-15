/********************************************************************
	created:	2014/03/23
	created:	23:3:2014   13:22
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\GameFonts.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	GameFonts
	file ext:	h
	author:		Stefano Musumeci

	purpose:	Header file containing all the game fonts.
*********************************************************************/
#ifndef GameFonts_h__
#define GameFonts_h__

#include "FontDescription.h"
#include "System.h"

extern FontDescription normalFont;
extern FontDescription outlineFont;

class TextRenderer
{
    public:
        TextRenderer(int backgroundId, int charblockId, int screenblockId, int priorityId);
        uint16_t* CreateTextArea(int x, int y, int width, int height);
        void ResetAreas();
        void ClearTextArea(uint16_t* areaPtr, int numBlocks, int startBlock = 0);

        template<int rowLength>
        void RenderString(FontDescription& font, uint16_t* startBlock, const char* text, uint32_t textLength, uint32_t yOffset, uint16_t fontColour, uint16_t outlineColour)
        {
            uint16_t colours[3];
            colours[0] = 0;
            colours[1] = outlineColour;
            colours[2] = fontColour;

            uint32_t xOffset = 0;

            for (int i = 0; i < textLength; i++)
            {
                unsigned char ch = text[i];
                GlyphDescription& glyph = font.glyphs[ch];
                const uint16_t* sourceCharacter = glyph.glyphData;

                uint16_t oddWidth = (glyph.width & 1);
                uint16_t evenWidth = (glyph.width - oddWidth) >> 1;

                for (uint32_t y = 0; y < glyph.height; y++)
                {
                    for (uint32_t x = 0; x < evenWidth; x++)
                    {
                        uint16_t colour = sourceCharacter[y * 4 + x];
                        if ( colour == 0 )
                            continue;
                        uint16_t pixelValue = colours[colour & 0xFF] | (colours[(colour & 0xFF00) >> 8] << 8);
                        hardware::PlotTile16<rowLength>(startBlock, xOffset + (x << 1), y + yOffset + glyph.yOffset, pixelValue);
                    }
                    if ( oddWidth )
                    {
                        uint16_t colour = sourceCharacter[y * 4 + evenWidth];
                        uint16_t pixelValue = colours[colour & 0xFF];
                        hardware::PlotTile8<rowLength>(startBlock, xOffset + (evenWidth << 1), y + yOffset + glyph.yOffset, pixelValue);
                    }
                }
                //Make sure xOffset is always an even number --
                xOffset += (glyph.xAdvance + 1) & ~1;
            }
        }

    private:
        static bool areFontsInitialized;
        hardware::Charblock* charblock;
        hardware::Screenblock* screenblock;
        int currentCharblock;
};


#endif // GameFonts_h__
