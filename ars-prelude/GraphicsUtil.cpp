#include "GraphicsUtil.h"
#include "System.h"
#include "gba.h"

#define MAX(a,b) a > b ? a : b
#define MIN(a,b) a < b ? a : b

namespace graphics
{
    uint16_t paletteBufferBg[256];
    uint16_t paletteBufferObj[256];

    void SetFadePaletteBackground( const uint16_t* palette )
    {
        hardware::CopyPalette(paletteBufferBg, palette);
    }

    void SetFadePaletteBackgroundEntry( uint16_t index, uint16_t colour )
    {
        paletteBufferBg[index] = colour;
    }

    void FadeOutBackground(int fadeSpeed)
    {
        hardware::CopyPalette(paletteBufferBg, (const uint16_t*)hardware::BG_Palette);
        for (int i = 0; i < 31; i++)
        {
            for (int k = 0; k < 256; k++)
            {
                uint16_t r, g, b;
                GetRGB(paletteBufferBg[k], &r, &g, &b);
                r -= MIN(r, i);
                g -= MIN(g, i);
                b -= MIN(b, i);
                hardware::BG_Palette[k] = RGB(r, g, b);
            }

            for (int j = 0; j < fadeSpeed; j++) // Wait for Vsync - speed
            {
                hardware::WaitVSync();
            }
        }
    }

    void FadeInBackground(int fadeSpeed)
    {
        for (int i = 0; i < 31; i++)
        {
            for (int k = 0; k < 256; k++)
            {
                uint16_t r, g, b;
                GetRGB(paletteBufferBg[k], &r, &g, &b);
                r = MIN(r, i);
                g = MIN(g, i);
                b = MIN(b, i);
                hardware::BG_Palette[k] = RGB(r, g, b);
            }

            for (int j = 0; j < fadeSpeed; j++) // Wait for Vsync - speed
            {
                hardware::WaitVSync();
            }
        }
    }

    void SetFadePaletteObjects( const uint16_t* palette )
    {
        hardware::CopyPalette(paletteBufferObj, palette);
    }

    void FadeOutObjects(int fadeSpeed)
    {
        hardware::CopyPalette(paletteBufferObj, (const uint16_t*)hardware::OBJ_Palette);
        for (int i = 0; i < 31; i++)
        {
            for (int k = 0; k < 256; k++)
            {
                uint16_t r, g, b;
                GetRGB(paletteBufferObj[k], &r, &g, &b);
                r -= MIN(r, i);
                g -= MIN(g, i);
                b -= MIN(b, i);
                hardware::OBJ_Palette[k] = RGB(r, g, b);
            }

            for (int j = 0; j < fadeSpeed; j++) // Wait for Vsync - speed
            {
                hardware::WaitVSync();
            }
        }
    }

    void FadeInObjects(int fadeSpeed)
    {
        for (int i = 0; i < 31; i++)
        {
            for (int k = 0; k < 256; k++)
            {
                uint16_t r, g, b;
                GetRGB(paletteBufferObj[k], &r, &g, &b);
                r = MIN(r, i);
                g = MIN(g, i);
                b = MIN(b, i);
                hardware::OBJ_Palette[k] = RGB(r, g, b);
            }

            for (int j = 0; j < fadeSpeed; j++) // Wait for Vsync - speed
            {
                hardware::WaitVSync();
            }
        }
    }

}
