#include "GameFonts.h"
#include "normal_font.h"
#include "outline_font.h"
#include "GraphicsUtil.h"
#include "System.h"
#include "gba.h"

bool isFontInitialized = false;
FontDescription normalFont;
FontDescription outlineFont;

template <uint16_t xTile, uint16_t yTile>
struct TilePointerOffset
{
    enum { Value = 32 * xTile + (32 * 28) * yTile };
};

void InitFonts()
{
    if ( isFontInitialized )
        return;

    normalFont.glyphs[' '] = GlyphDescription(normal_fontTiles + TilePointerOffset<0, 0>::Value, 0, 0, 3, 0);

    normalFont.glyphs['A'] = GlyphDescription(normal_fontTiles + TilePointerOffset<0, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['B'] = GlyphDescription(normal_fontTiles + TilePointerOffset<1, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['C'] = GlyphDescription(normal_fontTiles + TilePointerOffset<2, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['D'] = GlyphDescription(normal_fontTiles + TilePointerOffset<3, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['E'] = GlyphDescription(normal_fontTiles + TilePointerOffset<4, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['F'] = GlyphDescription(normal_fontTiles + TilePointerOffset<5, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['G'] = GlyphDescription(normal_fontTiles + TilePointerOffset<6, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['H'] = GlyphDescription(normal_fontTiles + TilePointerOffset<7, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['I'] = GlyphDescription(normal_fontTiles + TilePointerOffset<8, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['J'] = GlyphDescription(normal_fontTiles + TilePointerOffset<9, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['K'] = GlyphDescription(normal_fontTiles + TilePointerOffset<10, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['L'] = GlyphDescription(normal_fontTiles + TilePointerOffset<11, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['M'] = GlyphDescription(normal_fontTiles + TilePointerOffset<12, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['N'] = GlyphDescription(normal_fontTiles + TilePointerOffset<13, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['O'] = GlyphDescription(normal_fontTiles + TilePointerOffset<14, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['P'] = GlyphDescription(normal_fontTiles + TilePointerOffset<15, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['Q'] = GlyphDescription(normal_fontTiles + TilePointerOffset<16, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['R'] = GlyphDescription(normal_fontTiles + TilePointerOffset<17, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['S'] = GlyphDescription(normal_fontTiles + TilePointerOffset<18, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['T'] = GlyphDescription(normal_fontTiles + TilePointerOffset<19, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['U'] = GlyphDescription(normal_fontTiles + TilePointerOffset<20, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['V'] = GlyphDescription(normal_fontTiles + TilePointerOffset<21, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['W'] = GlyphDescription(normal_fontTiles + TilePointerOffset<22, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['X'] = GlyphDescription(normal_fontTiles + TilePointerOffset<23, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['Y'] = GlyphDescription(normal_fontTiles + TilePointerOffset<24, 0>::Value, 6, 8, 5, 0);
    normalFont.glyphs['Z'] = GlyphDescription(normal_fontTiles + TilePointerOffset<25, 0>::Value, 6, 8, 5, 0);

    normalFont.glyphs['a'] = GlyphDescription(normal_fontTiles + TilePointerOffset<0, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['b'] = GlyphDescription(normal_fontTiles + TilePointerOffset<1, 1>::Value, 6, 8, 5, 0);
    normalFont.glyphs['c'] = GlyphDescription(normal_fontTiles + TilePointerOffset<2, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['d'] = GlyphDescription(normal_fontTiles + TilePointerOffset<3, 1>::Value, 6, 8, 5, 0);
    normalFont.glyphs['e'] = GlyphDescription(normal_fontTiles + TilePointerOffset<4, 1>::Value, 6, 8, 6, 2);
    normalFont.glyphs['f'] = GlyphDescription(normal_fontTiles + TilePointerOffset<5, 1>::Value, 4, 8, 3, 0);
    normalFont.glyphs['g'] = GlyphDescription(normal_fontTiles + TilePointerOffset<6, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['h'] = GlyphDescription(normal_fontTiles + TilePointerOffset<7, 1>::Value, 6, 8, 5, 0);
    normalFont.glyphs['i'] = GlyphDescription(normal_fontTiles + TilePointerOffset<8, 1>::Value, 6, 8, 2, 0);
    normalFont.glyphs['j'] = GlyphDescription(normal_fontTiles + TilePointerOffset<9, 1>::Value, 6, 8, 4, 0);
    normalFont.glyphs['k'] = GlyphDescription(normal_fontTiles + TilePointerOffset<10, 1>::Value, 6, 8, 4, 0);
    normalFont.glyphs['l'] = GlyphDescription(normal_fontTiles + TilePointerOffset<11, 1>::Value, 3, 8, 3, 1);
    normalFont.glyphs['m'] = GlyphDescription(normal_fontTiles + TilePointerOffset<12, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['n'] = GlyphDescription(normal_fontTiles + TilePointerOffset<13, 1>::Value, 6, 8, 4, 2);
    normalFont.glyphs['o'] = GlyphDescription(normal_fontTiles + TilePointerOffset<14, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['p'] = GlyphDescription(normal_fontTiles + TilePointerOffset<15, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['q'] = GlyphDescription(normal_fontTiles + TilePointerOffset<16, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['r'] = GlyphDescription(normal_fontTiles + TilePointerOffset<17, 1>::Value, 6, 8, 4, 2);
    normalFont.glyphs['s'] = GlyphDescription(normal_fontTiles + TilePointerOffset<18, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['t'] = GlyphDescription(normal_fontTiles + TilePointerOffset<19, 1>::Value, 4, 8, 4, 1);
    normalFont.glyphs['u'] = GlyphDescription(normal_fontTiles + TilePointerOffset<20, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['v'] = GlyphDescription(normal_fontTiles + TilePointerOffset<21, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['w'] = GlyphDescription(normal_fontTiles + TilePointerOffset<22, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['x'] = GlyphDescription(normal_fontTiles + TilePointerOffset<23, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['y'] = GlyphDescription(normal_fontTiles + TilePointerOffset<24, 1>::Value, 6, 8, 5, 2);
    normalFont.glyphs['z'] = GlyphDescription(normal_fontTiles + TilePointerOffset<25, 1>::Value, 6, 8, 5, 2);

    normalFont.glyphs['0'] = GlyphDescription(normal_fontTiles + TilePointerOffset<0, 2>::Value, 6, 8, 5, 0);
    normalFont.glyphs['1'] = GlyphDescription(normal_fontTiles + TilePointerOffset<1, 2>::Value, 4, 8, 3, 0);
    normalFont.glyphs['2'] = GlyphDescription(normal_fontTiles + TilePointerOffset<2, 2>::Value, 6, 8, 5, 0);
    normalFont.glyphs['3'] = GlyphDescription(normal_fontTiles + TilePointerOffset<3, 2>::Value, 6, 8, 5, 0);
    normalFont.glyphs['4'] = GlyphDescription(normal_fontTiles + TilePointerOffset<4, 2>::Value, 6, 8, 5, 0);
    normalFont.glyphs['5'] = GlyphDescription(normal_fontTiles + TilePointerOffset<5, 2>::Value, 6, 8, 5, 0);
    normalFont.glyphs['6'] = GlyphDescription(normal_fontTiles + TilePointerOffset<6, 2>::Value, 6, 8, 5, 0);
    normalFont.glyphs['7'] = GlyphDescription(normal_fontTiles + TilePointerOffset<7, 2>::Value, 6, 8, 5, 0);
    normalFont.glyphs['8'] = GlyphDescription(normal_fontTiles + TilePointerOffset<8, 2>::Value, 6, 8, 5, 0);
    normalFont.glyphs['9'] = GlyphDescription(normal_fontTiles + TilePointerOffset<9, 2>::Value, 6, 8, 5, 0);
    normalFont.glyphs['!'] = GlyphDescription(normal_fontTiles + TilePointerOffset<13, 2>::Value, 2, 8, 2, 0);
    normalFont.glyphs['?'] = GlyphDescription(normal_fontTiles + TilePointerOffset<12, 2>::Value, 6, 8, 5, 0);

    normalFont.glyphs['+'] = GlyphDescription(normal_fontTiles + TilePointerOffset<16, 2>::Value, 7, 8, 5, 0);
    normalFont.glyphs['-'] = GlyphDescription(normal_fontTiles + TilePointerOffset<17, 2>::Value, 7, 8, 5, 0);
    normalFont.glyphs['/'] = GlyphDescription(normal_fontTiles + TilePointerOffset<18, 2>::Value, 7, 8, 5, 0);
    normalFont.glyphs['%'] = GlyphDescription(normal_fontTiles + TilePointerOffset<22, 2>::Value, 7, 8, 5, 0);
    normalFont.glyphs[':'] = GlyphDescription(normal_fontTiles + TilePointerOffset<19, 2>::Value, 7, 8, 2, 2);
    normalFont.glyphs['('] = GlyphDescription(normal_fontTiles + TilePointerOffset<14, 2>::Value, 7, 8, 5, 0);
    normalFont.glyphs[')'] = GlyphDescription(normal_fontTiles + TilePointerOffset<15, 2>::Value, 7, 8, 5, 0);
    normalFont.glyphs[','] = GlyphDescription(normal_fontTiles + TilePointerOffset<11, 2>::Value, 7, 8, 4, 7);
    normalFont.glyphs['.'] = GlyphDescription(normal_fontTiles + TilePointerOffset<10, 2>::Value, 7, 8, 4, 6);
    normalFont.glyphs['\''] = GlyphDescription(normal_fontTiles + TilePointerOffset<11, 2>::Value, 7, 8, 4, 1);
    normalFont.glyphs['"'] = GlyphDescription(normal_fontTiles + TilePointerOffset<11, 2>::Value, 7, 8, 4, 1);
    normalFont.glyphs['&'] = GlyphDescription(normal_fontTiles + TilePointerOffset<26, 0>::Value, 7, 8, 4, 1);
    normalFont.glyphs['*'] = GlyphDescription(normal_fontTiles + TilePointerOffset<23, 2>::Value, 7, 8, 6, 1);

    outlineFont.glyphs[' '] = GlyphDescription(outline_fontTiles + TilePointerOffset<0, 0>::Value, 0, 0, 4, 0);

    outlineFont.glyphs['A'] = GlyphDescription(outline_fontTiles + TilePointerOffset<0, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['B'] = GlyphDescription(outline_fontTiles + TilePointerOffset<1, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['C'] = GlyphDescription(outline_fontTiles + TilePointerOffset<2, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['D'] = GlyphDescription(outline_fontTiles + TilePointerOffset<3, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['E'] = GlyphDescription(outline_fontTiles + TilePointerOffset<4, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['F'] = GlyphDescription(outline_fontTiles + TilePointerOffset<5, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['G'] = GlyphDescription(outline_fontTiles + TilePointerOffset<6, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['H'] = GlyphDescription(outline_fontTiles + TilePointerOffset<7, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['I'] = GlyphDescription(outline_fontTiles + TilePointerOffset<8, 0>::Value, 6, 7, 4, 0);
    outlineFont.glyphs['J'] = GlyphDescription(outline_fontTiles + TilePointerOffset<9, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['K'] = GlyphDescription(outline_fontTiles + TilePointerOffset<10, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['L'] = GlyphDescription(outline_fontTiles + TilePointerOffset<11, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['M'] = GlyphDescription(outline_fontTiles + TilePointerOffset<12, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['N'] = GlyphDescription(outline_fontTiles + TilePointerOffset<13, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['O'] = GlyphDescription(outline_fontTiles + TilePointerOffset<14, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['P'] = GlyphDescription(outline_fontTiles + TilePointerOffset<15, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['Q'] = GlyphDescription(outline_fontTiles + TilePointerOffset<16, 0>::Value, 6, 8, 5, 0);
    outlineFont.glyphs['R'] = GlyphDescription(outline_fontTiles + TilePointerOffset<17, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['S'] = GlyphDescription(outline_fontTiles + TilePointerOffset<18, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['T'] = GlyphDescription(outline_fontTiles + TilePointerOffset<19, 0>::Value, 7, 7, 4, 0);
    outlineFont.glyphs['U'] = GlyphDescription(outline_fontTiles + TilePointerOffset<20, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['V'] = GlyphDescription(outline_fontTiles + TilePointerOffset<21, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['W'] = GlyphDescription(outline_fontTiles + TilePointerOffset<22, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['X'] = GlyphDescription(outline_fontTiles + TilePointerOffset<23, 0>::Value, 6, 7, 5, 0);
    outlineFont.glyphs['Y'] = GlyphDescription(outline_fontTiles + TilePointerOffset<24, 0>::Value, 6, 7, 4, 0);
    outlineFont.glyphs['Z'] = GlyphDescription(outline_fontTiles + TilePointerOffset<25, 0>::Value, 6, 7, 5, 0);

    for (unsigned char c = 'a'; c <= 'z'; c++ )
    {
        outlineFont.glyphs[c] = outlineFont.glyphs['A' + c - 'a'];
    }

    outlineFont.glyphs['0'] = GlyphDescription(outline_fontTiles + TilePointerOffset<0, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['1'] = GlyphDescription(outline_fontTiles + TilePointerOffset<1, 1>::Value, 7, 7, 4, 0);
    outlineFont.glyphs['2'] = GlyphDescription(outline_fontTiles + TilePointerOffset<2, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['3'] = GlyphDescription(outline_fontTiles + TilePointerOffset<3, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['4'] = GlyphDescription(outline_fontTiles + TilePointerOffset<4, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['5'] = GlyphDescription(outline_fontTiles + TilePointerOffset<5, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['6'] = GlyphDescription(outline_fontTiles + TilePointerOffset<6, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['7'] = GlyphDescription(outline_fontTiles + TilePointerOffset<7, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['8'] = GlyphDescription(outline_fontTiles + TilePointerOffset<8, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['9'] = GlyphDescription(outline_fontTiles + TilePointerOffset<9, 1>::Value, 7, 7, 5, 0);

    outlineFont.glyphs['/'] = GlyphDescription(outline_fontTiles + TilePointerOffset<22, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['%'] = GlyphDescription(outline_fontTiles + TilePointerOffset<25, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs[':'] = GlyphDescription(outline_fontTiles + TilePointerOffset<15, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['('] = GlyphDescription(outline_fontTiles + TilePointerOffset<23, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs[')'] = GlyphDescription(outline_fontTiles + TilePointerOffset<24, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['+'] = GlyphDescription(outline_fontTiles + TilePointerOffset<20, 1>::Value, 7, 7, 5, 0);
    outlineFont.glyphs['-'] = GlyphDescription(outline_fontTiles + TilePointerOffset<21, 1>::Value, 7, 7, 5, 0);
}

bool TextRenderer::areFontsInitialized = false;

TextRenderer::TextRenderer( int backgroundId, int charblockId, int screenblockId, int priorityId )
{
    if (areFontsInitialized == false)
    {
        InitFonts();
        areFontsInitialized = true;
    }
    hardware::SetupBackground(backgroundId, charblockId, screenblockId, false, priorityId);
    this->charblock = &hardware::Charblocks[charblockId];
    this->screenblock = &hardware::Screenblocks[screenblockId];
    this->currentCharblock = 1;

    hardware::BG_Palette[253] = RGB(31, 31, 31);
    hardware::BG_Palette[254] = RGB(8, 6, 4);
    hardware::BG_Palette[255] = RGB(20, 16, 11);

    graphics::SetFadePaletteBackgroundEntry(253, hardware::BG_Palette[253]);
    graphics::SetFadePaletteBackgroundEntry(254, hardware::BG_Palette[254]);
    graphics::SetFadePaletteBackgroundEntry(255, hardware::BG_Palette[255]);

    for (int i = 0; i < 8192; i++)
        charblock->data[i] = 0;

    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            screenblock->SetTile(i, j, 0);
        }
    }
}

uint16_t* TextRenderer::CreateTextArea( int x, int y, int width, int height )
{
    uint16_t* startCharblock = charblock->GetTile8(currentCharblock);
    for (int j = y; j < y + height; j++)
    {
        for (int i = x; i < x + width; i++)
        {
            screenblock->SetTile(i, j, currentCharblock++);
        }
    }
    return startCharblock;
}

void TextRenderer::ClearTextArea( uint16_t* areaPtr, int numBlocks, int startBlock /*= 0*/ )
{
    uint32_t* start = (uint32_t*)areaPtr + (startBlock * 16);
    uint32_t* end = start + (numBlocks * 16);
    while (start != end)
    {
        *start++ = 0;
    }
}

void TextRenderer::ResetAreas()
{
    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            screenblock->SetTile(i, j, 0);
        }
    }
    for (int i = 0; i < 8192; i++)
        charblock->data[i] = 0;
    currentCharblock = 1;
}

