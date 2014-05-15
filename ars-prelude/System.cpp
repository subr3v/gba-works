#include "System.h"
#include "gba.h"
#include <cstdlib>
#include <cstdio>
#include <stdarg.h>

#define TM_FREQ_SYS          0  //!< System clock timer (16.7 Mhz)
#define TM_FREQ_1            0  //!< 1 cycle/tick (16.7 Mhz)
#define TM_FREQ_64      0x0001  //!< 64 cycles/tick (262 kHz)
#define TM_FREQ_256     0x0002  //!< 256 cycles/tick (66 kHz)
#define TM_FREQ_1024    0x0003  //!< 1024 cycles/tick (16 kHz)
#define TM_CASCADE      0x0004  //!< Increment when preceding timer overflows
#define TM_IRQ          0x0040  //!< Enable timer irq
#define TM_ENABLE       0x0080  //!< Enable timer

unsigned long marsenne_twister_random();
void marsenne_twister_init(int seed);

int Random( int min, int max )
{
    return marsenne_twister_random() % (max - min) + min;
}

void RandomSeed( int seed )
{
    marsenne_twister_init(seed);
}

namespace hardware
{
    volatile uint16_t* BG_Palette = REG_PALETTE_BG;
    volatile Palette16* BG_Palette16 = (Palette16*)REG_PALETTE_BG;
    volatile uint16_t* OBJ_Palette = REG_PALETTE_OBJ;
    volatile Palette16* OBJ_Palette16 = (Palette16*)REG_PALETTE_OBJ;
    Screenblock* Screenblocks = (Screenblock*)REG_VIDEO_BASE;
    Charblock* Charblocks = (Charblock*)REG_VIDEO_BASE;

    uint32_t lastProfileTime = 0;
    uint16_t currKeyReg;
    uint16_t lastKeyReg;
    static const uint16_t KEY_MASK = 0x03FF; // Key mask used to extract input data from gba's input registers

    void UpdateInput()
    {
        lastKeyReg = currKeyReg;
        currKeyReg = ~REG_KEYINPUT & KEY_MASK;
    }

    void ProfileStart()
    {
        /*
        REG_TM2D = 0;
        REG_TM3D = 0;
        REG_TM2CNT = 0;
        REG_TM3CNT = 0;
        REG_TM3CNT = TM_ENABLE | TM_CASCADE;
        REG_TM2CNT = TM_ENABLE;
        */
    }

    uint32_t ProfileEnd()
    {
        /*
        REG_TM2CNT = 0;
        lastProfileTime = (REG_TM3D << 16) | REG_TM2D;
        */
        return lastProfileTime;
    }

    uint32_t GetLastProfileTime()
    {
        return lastProfileTime;
    }

    bool IsButtonDown( int button )
    {
        return (currKeyReg & button);
    }

    bool IsButtonPressed( int button )
    {
        return (currKeyReg & button) && (~lastKeyReg & button);
    }

    void WaitVSync()
    {
        while (REG_VCOUNT >= 160);  // wait till VDraw
        while (REG_VCOUNT < 160);   // wait till VBlank
    }

    void SetupDisplayControl( bool EnableOBJ, bool EnableBG0, bool EnableBG1, bool EnableBG2, bool EnableBG3 )
    {
        int DisplayControl = DCNT_MODE0;
        DisplayControl |= EnableOBJ ? DCNT_OBJ : 0;
        DisplayControl |= EnableBG0 ? DCNT_BG0 : 0;
        DisplayControl |= EnableBG1 ? DCNT_BG1 : 0;
        DisplayControl |= EnableBG2 ? DCNT_BG2 : 0;
        DisplayControl |= EnableBG3 ? DCNT_BG3 : 0;
        REG_DISPCNT = DisplayControl;
    }

    void SetupBackground( int backgroundId, int charblockNumber, int screenblockNumber, bool use4bppMode, int priorityId )
    {
        volatile uint16_t* backgroundRegister = REGISTER_PTR(uint16_t, 0x4000008 + sizeof(uint16_t) * backgroundId);
        *backgroundRegister = BG_PRIO(priorityId) | BG_REG_32x32 | BG_CBB(charblockNumber) | BG_SBB(screenblockNumber) | (use4bppMode ? BG_4BPP : BG_8BPP);
        SetupBackgroundScroll(backgroundId, 0, 0); // Resets the background scroll.
    }

    void SetupBackgroundScroll( uint32_t backgroundId, uint16_t x, uint16_t y )
    {
        REGISTER(uint16_t, 0x4000010 + sizeof(uint16_t) * backgroundId) = x;
        REGISTER(uint16_t, 0x4000012 + sizeof(uint16_t) * backgroundId) = y;
    }

    void CopyScreenBlock( Screenblock* screenBlock, int screenblockNumber )
    {
        uint32_t* srcPtr = (uint32_t*)screenBlock->tiles;
        uint32_t* dstPtr = (uint32_t*)Screenblocks[screenblockNumber].tiles;

        uint32_t* endPtr = (uint32_t*)Screenblocks[screenblockNumber + 1].tiles;

        while ( dstPtr < endPtr )
        {
            *(dstPtr + 0) = *(srcPtr + 0);
            *(dstPtr + 1) = *(srcPtr + 1);
            *(dstPtr + 2) = *(srcPtr + 2);
            *(dstPtr + 3) = *(srcPtr + 3);
            *(dstPtr + 4) = *(srcPtr + 4);
            *(dstPtr + 5) = *(srcPtr + 5);
            *(dstPtr + 6) = *(srcPtr + 6);
            *(dstPtr + 7) = *(srcPtr + 7);
            dstPtr += 8;
            srcPtr += 8;
        }
    }

    void CopyTile4( uint16_t* sourceData, uint16_t* destinationData )
    {
        uint32_t* src = (uint32_t*)sourceData;
        uint32_t* dst = (uint32_t*)destinationData;
        *(dst + 0) = *(src + 0);
        *(dst + 1) = *(src + 1);
        *(dst + 2) = *(src + 2);
        *(dst + 3) = *(src + 3);
        *(dst + 4) = *(src + 4);
        *(dst + 5) = *(src + 5);
        *(dst + 6) = *(src + 6);
        *(dst + 7) = *(src + 7);
    }

    void CopyTile4From8( uint16_t* sourceData, uint16_t* destinationData )
    {
        uint8_t* src = (uint8_t*)sourceData;
        destinationData[0] = ( src[0] | (src[1] << 4) ) | ( (src[2] | (src[3] << 4))  << 8);
        destinationData[1] = ( src[4] | (src[5] << 4) ) | ( (src[6] | (src[7] << 4))  << 8);
        destinationData[2] = ( src[8] | (src[9] << 4) ) | ( (src[10] | (src[11] << 4))  << 8);
        destinationData[3] = ( src[12] | (src[13] << 4) ) | ( (src[14] | (src[15] << 4))  << 8);
        destinationData[4] = ( src[16] | (src[17] << 4) ) | ( (src[18] | (src[19] << 4))  << 8);
        destinationData[5] = ( src[20] | (src[21] << 4) ) | ( (src[22] | (src[23] << 4))  << 8);
        destinationData[6] = ( src[24] | (src[25] << 4) ) | ( (src[26] | (src[27] << 4))  << 8);
        destinationData[7] = ( src[28] | (src[29] << 4) ) | ( (src[30] | (src[31] << 4))  << 8);
        destinationData[8] = ( src[32] | (src[33] << 4) ) | ( (src[34] | (src[35] << 4))  << 8);
        destinationData[9] = ( src[36] | (src[37] << 4) ) | ( (src[38] | (src[39] << 4))  << 8);
        destinationData[10] = ( src[40] | (src[41] << 4) ) | ( (src[42] | (src[43] << 4))  << 8);
        destinationData[11] = ( src[44] | (src[45] << 4) ) | ( (src[46] | (src[39] << 4))  << 8);
        destinationData[12] = ( src[48] | (src[49] << 4) ) | ( (src[50] | (src[51] << 4))  << 8);
        destinationData[13] = ( src[52] | (src[53] << 4) ) | ( (src[54] | (src[55] << 4))  << 8);
        destinationData[14] = ( src[56] | (src[57] << 4) ) | ( (src[58] | (src[59] << 4))  << 8);
        destinationData[15] = ( src[60] | (src[61] << 4) ) | ( (src[62] | (src[63] << 4))  << 8);
    }

    void CopyTile8( uint16_t* sourceData, uint16_t* destinationData )
    {
        *(destinationData + 0) = *(sourceData + 0);
        *(destinationData + 1) = *(sourceData + 1);
        *(destinationData + 2) = *(sourceData + 2);
        *(destinationData + 3) = *(sourceData + 3);
        *(destinationData + 4) = *(sourceData + 4);
        *(destinationData + 5) = *(sourceData + 5);
        *(destinationData + 6) = *(sourceData + 6);
        *(destinationData + 7) = *(sourceData + 7);
        *(destinationData + 8) = *(sourceData + 8);
        *(destinationData + 9) = *(sourceData + 9);
        *(destinationData + 10) = *(sourceData + 10);
        *(destinationData + 11) = *(sourceData + 11);
        *(destinationData + 12) = *(sourceData + 12);
        *(destinationData + 13) = *(sourceData + 13);
        *(destinationData + 14) = *(sourceData + 14);
        *(destinationData + 15) = *(sourceData + 15);
        *(destinationData + 16) = *(sourceData + 16);
        *(destinationData + 17) = *(sourceData + 17);
        *(destinationData + 18) = *(sourceData + 18);
        *(destinationData + 19) = *(sourceData + 19);
        *(destinationData + 20) = *(sourceData + 20);
        *(destinationData + 21) = *(sourceData + 21);
        *(destinationData + 22) = *(sourceData + 22);
        *(destinationData + 23) = *(sourceData + 23);
        *(destinationData + 24) = *(sourceData + 24);
        *(destinationData + 25) = *(sourceData + 25);
        *(destinationData + 26) = *(sourceData + 26);
        *(destinationData + 27) = *(sourceData + 27);
        *(destinationData + 28) = *(sourceData + 28);
        *(destinationData + 29) = *(sourceData + 29);
        *(destinationData + 30) = *(sourceData + 30);
        *(destinationData + 31) = *(sourceData + 31);
    }

    char errorMessageBuffer[1024];

    void printErrorMessage( char* msg, ... )
    {
        va_list va;
        va_start(va, msg);
        vsprintf(errorMessageBuffer, msg, va);
        va_end(va);
    }

    char* getLastErrorMessage()
    {
        return errorMessageBuffer;
    }

    void CopyPalette( volatile uint16_t* targetPalette, const uint16_t* sourcePalette )
    {
        for (int i = 0; i < 256; i++)
            targetPalette[i] = sourcePalette[i];
    }

    void FillPalette( volatile uint16_t* targetPalette, uint16_t colour )
    {
        for (int i = 0; i < 256; i++)
            targetPalette[i] = colour;
    }

    void SetSprite8( int spriteIndex, uint16_t x, uint16_t y, uint16_t charblockId, uint16_t shape, uint16_t size, bool flipHorizontal /*= false*/, bool flipVertical /*= false*/, uint16_t bgPriority /*= 0*/ )
    {
        SetObject(spriteIndex,
                  ATTR0_SHAPE(shape) | ATTR0_8BPP | ATTR0_REG | ATTR0_Y(y),
                  ATTR1_SIZE(size) | ATTR1_X(x) | (flipHorizontal ? ATTR1_HFLIP : 0) | (flipVertical ? ATTR1_VFLIP : 0),
                  ATTR2_ID8(charblockId) | ATTR2_PRIO(bgPriority));
    }

    void HideSprite( int spriteIndex )
    {
        SetObject(spriteIndex, ATTR0_HIDE, 0, 0);
    }

    void SetSprite4( int spriteIndex, uint16_t x, uint16_t y, uint16_t charblockId, uint16_t paletteId, uint16_t shape, uint16_t size, bool flipHorizontal /*= false*/, bool flipVertical /*= false*/, uint16_t bgPriority /*= 0*/ )
    {
        SetObject(spriteIndex,
                  ATTR0_SHAPE(shape) | ATTR0_4BPP | ATTR0_REG | ATTR0_Y(y),
                  ATTR1_SIZE(size) | ATTR1_X(x) | (flipHorizontal ? ATTR1_HFLIP : 0) | (flipVertical ? ATTR1_VFLIP : 0),
                  ATTR2_ID4(charblockId) | ATTR2_PALBANK(paletteId) | ATTR2_PRIO(bgPriority));
    }

    void LoadCharblock( int charlockIndex, const uint16_t* data )
    {
        for (int i = 0; i < 8192; i++)
        {
            Charblocks[charlockIndex].data[i] = data[i];
        }
    }

}

#define MT_LEN 624

int mt_index;
unsigned long mt_buffer[MT_LEN];

void marsenne_twister_init(int seed)
{
    srand(seed);
    int i;
    for (i = 0; i < MT_LEN; i++)
        mt_buffer[i] = rand();
    mt_index = 0;
}

#define MT_IA           397
#define MT_IB           (MT_LEN - MT_IA)
#define UPPER_MASK      0x80000000
#define LOWER_MASK      0x7FFFFFFF
#define MATRIX_A        0x9908B0DF
#define TWIST(b,i,j)    ((b)[i] & UPPER_MASK) | ((b)[j] & LOWER_MASK)
#define MAGIC(s)        (((s)&1)*MATRIX_A)

unsigned long marsenne_twister_random()
{
    unsigned long* b = mt_buffer;
    int idx = mt_index;
    unsigned long s;
    int i;

    if (idx == MT_LEN * sizeof(unsigned long))
    {
        idx = 0;
        i = 0;
        for (; i < MT_IB; i++)
        {
            s = TWIST(b, i, i + 1);
            b[i] = b[i + MT_IA] ^ (s >> 1) ^ MAGIC(s);
        }
        for (; i < MT_LEN - 1; i++)
        {
            s = TWIST(b, i, i + 1);
            b[i] = b[i - MT_IB] ^ (s >> 1) ^ MAGIC(s);
        }

        s = TWIST(b, MT_LEN - 1, 0);
        b[MT_LEN - 1] = b[MT_IA - 1] ^ (s >> 1) ^ MAGIC(s);
    }
    mt_index = idx + sizeof(unsigned long);
    return *(unsigned long*)((unsigned char*)b + idx);
    /* Here there is a commented out block in MB's original program */
}
