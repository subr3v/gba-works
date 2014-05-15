/********************************************************************
    created:    2014/02/21
    created:    21:2:2014   16:40
    filename:   C:\Users\Stefano\Desktop\uni-repo\gba-semester2\GbaProject\Engine.h
    file path:  C:\Users\Stefano\Desktop\uni-repo\gba-semester2\GbaProject
    file base:  Engine
    file ext:   h
    author:     Stefano Musumeci

    purpose:    System header, encapsulate low level behavior and makes it easier to access all the gba features.
*********************************************************************/
#ifndef System_h__
#define System_h__

#include <stdint.h>

static inline uint16_t BG_TILE(uint16_t tileIndex, uint16_t paletteIndex = 0, bool flipHorizontal = false, bool flipVertical = false)
{
    return tileIndex | (paletteIndex << 12) | (flipHorizontal << 10) | (flipVertical << 11);
}

// Construct an RGB colour value
static inline void GetRGB(uint16_t colour, uint16_t* r, uint16_t* g, uint16_t* b)
{
    *r = colour & 31;
    *g = (colour >> 5) & 31;
    *b = (colour >> 10) & 31;
}

static inline uint16_t GetTileID(uint16_t x, uint16_t y)
{
    return y * 16 + x;
}

template<int X, int Y>
struct TileID
{
    enum { Value = Y * 16 + X };
};

/*
*   Returns a random number (will be implemented with marsenne twister eventually)
*/
int Random(int min, int max);
void RandomSeed(int seed);

namespace hardware
{
    void printErrorMessage(char* msg, ...);
    char* getLastErrorMessage();

    struct Screenblock
    {
        uint16_t tiles[1024];

        void SetTile(uint32_t x, uint32_t y, uint16_t tile)
        {
            tiles[y * 32 + x] = tile;
        }
    };

    struct Palette16
    {
        uint16_t entries[16];
    };

    struct Charblock
    {
        uint16_t data[8192];
        uint16_t* GetTile4(uint32_t tileNumber)
        {
            return &data[tileNumber * 16];
        }
        uint16_t* GetTile8(uint32_t tileNumber)
        {
            return &data[tileNumber * 32];
        }
    };

    extern volatile uint16_t* BG_Palette;
    extern volatile Palette16* BG_Palette16;
    extern volatile uint16_t* OBJ_Palette;
    extern volatile Palette16* OBJ_Palette16;
    extern Screenblock* Screenblocks;
    extern Charblock* Charblocks;

    /*
    *   Setups display control ( defaults to mode 0, and 2d obj mapping if obj is enabled )
    */
    void SetupDisplayControl(bool EnableOBJ, bool EnableBG0, bool EnableBG1, bool EnableBG2, bool EnableBG3);

    /*
    *   Setup hardware register for a specific background, you can specify the charblock and screenblock number and if you're using 4bpp or 8bpp tiles
    */
    void SetupBackground(int backgroundId, int charblockNumber, int screenblockNumber, bool use4bppMode, int priorityId);

    /*
    *   Setups the x and y scrolling coord for a specific background
    */
    void SetupBackgroundScroll(uint32_t backgroundId, uint16_t x, uint16_t y);

    void HideSprite(int spriteIndex);

    void SetSprite8(int spriteIndex, uint16_t x, uint16_t y, uint16_t charblockId, uint16_t shape, uint16_t size, bool flipHorizontal = false, bool flipVertical = false, uint16_t bgPriority = 0);
    void SetSprite4(int spriteIndex, uint16_t x, uint16_t y, uint16_t charblockId, uint16_t paletteId, uint16_t shape, uint16_t size, bool flipHorizontal = false, bool flipVertical = false, uint16_t bgPriority = 0);

    /*
    * Copies a screenblock to VRAM
    */
    void CopyScreenBlock(Screenblock* screenBlock, int screenblockNumber);

    /*
    *  Copies tile from system ram to VRAM ( destination data must be part of a charblock )
    */
    void CopyTile4From8(uint16_t* sourceData, uint16_t* destinationData);

    /*
    *  Copies tile from system ram to VRAM ( destination data must be part of a charblock )
    */
    void CopyTile4(uint16_t* sourceData, uint16_t* destinationData);

    /*
    *  Copies tile from system ram to VRAM ( destination data must be part of a charblock )
    */
    void CopyTile8(uint16_t* sourceData, uint16_t* destinationData);

    /*
    * Copies a whole 256 palette from source to target ( target should be in VRAM )
    */
    void CopyPalette(volatile uint16_t* targetPalette, const uint16_t* sourcePalette);

    void LoadCharblock(int charlockIndex, const uint16_t* data);
    /*
    * Fill a whole 256 palette with a single colour ( target should be in VRAM )
    */
    void FillPalette(volatile uint16_t* targetPalette, uint16_t colour);

    template <int tilesPerRow>
    static inline void PlotTile8(uint16_t* dstTile, uint32_t dstX, uint32_t dstY, uint16_t col)
    {
        uint16_t dstIndex = (32 * (dstX >> 3)) + (32 * tilesPerRow) * (dstY >> 3) + ((dstY & 7) << 2) + ((dstX & 7) >> 1);
        if ( (dstX & 1) == 0 )
            dstTile[dstIndex] = col | (dstTile[dstIndex] & 0xFF00);
        else
            dstTile[dstIndex] = (dstTile[dstIndex] & 0x00FF) | (col << 8);
    }

    template <int tilesPerRow>
    static inline void PlotTile16(uint16_t* dstTile, uint32_t dstX, uint32_t dstY, uint16_t col)
    {
        uint16_t dstIndex = (32 * (dstX >> 3)) + (32 * tilesPerRow) * (dstY >> 3) + ((dstY & 7) << 2) + ((dstX & 7) >> 1);
        dstTile[dstIndex] = col;
    }

    /*
    *  Updates input status by reading from hardware registers
    */
    void UpdateInput();

    /*
    *   Returns true if button is currently held down, false otherwise.
    */
    bool IsButtonDown(int button);

    /*
    *   Returns true if button is being pressed in the current frame, false otherwise.
    */
    bool IsButtonPressed(int button);

    /*
    *   Wait for Vsync to happen
    */
    void WaitVSync();

}

#endif // System_h__
