#include "ObjectGraphics.h"
#include "sprites_sheet.h"
#include "System.h"
#include "GraphicsUtil.h"

SpriteDescription objectSprites[SpriteIdentifier_Count];
AnimationDescription spriteAnimationFrames[Animation_Count];

const uint32_t stillUnitFrames[1] = { 1 };
const uint32_t stillFrames[1] = { 0 };
const uint32_t hurtFrames[1] = { 8 };
const uint32_t walkFrames[5] = { 0, 0, 1, 2, 2 };
const uint32_t idleFrames[2] = { 1, 9 };
const uint32_t attackFrames[5] = { 3, 4, 5, 5, 6 };
const uint32_t sleepFrames[2] = { 11, 12 };
const uint32_t injuredFrames[2] = { 10 };
const uint32_t specialFrames[1] = { 7 };
const uint32_t triggerFrames[14] = { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 };

const uint16_t* GetTiles(uint16_t x, uint16_t rowNumber)
{
    const int TILE_SIZE = 32 * 4;
    const int SPRITESHEET_STRIDE = TILE_SIZE * 52;
    return sprites_sheetTiles + SPRITESHEET_STRIDE * rowNumber + x * TILE_SIZE;
}

void LoadUnitGraphics()
{
    const int UNIT_CENTER_X_OFFSET = -5;
    const int UNIT_CENTER_Y_OFFSET = -18;

    objectSprites[Sprite_Knight] = SpriteDescription(GetTiles(0, 5), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Mage] = SpriteDescription(GetTiles(0, 8), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Thief] = SpriteDescription(GetTiles(0, 11), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Slime] = SpriteDescription(GetTiles(0, 14), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Fire_Wisp] = SpriteDescription(GetTiles(0, 17), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Wererat] = SpriteDescription(GetTiles(0, 20), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Slime_Yellow] = SpriteDescription(GetTiles(0, 23), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Slime_Green] = SpriteDescription(GetTiles(0, 26), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Skeleton_Thief] = SpriteDescription(GetTiles(0, 29), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Skeleton_Mage] = SpriteDescription(GetTiles(0, 32), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Skeleton_Warrior] = SpriteDescription(GetTiles(0, 35), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Ice_Wisp] = SpriteDescription(GetTiles(0, 38), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Brown_Wisp] = SpriteDescription(GetTiles(0, 41), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Destruction_Wisp] = SpriteDescription(GetTiles(0, 44), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Shadow] = SpriteDescription(GetTiles(0, 47), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Wererat_Blue] = SpriteDescription(GetTiles(0, 50), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Wererat_Red] = SpriteDescription(GetTiles(0, 53), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);
    objectSprites[Sprite_Wyvern] = SpriteDescription(GetTiles(0, 56), UNIT_CENTER_X_OFFSET, UNIT_CENTER_Y_OFFSET, true);

    objectSprites[Sprite_Gold] = SpriteDescription(GetTiles(1, 0), -4, -2, false);
    objectSprites[Sprite_Food_Slime] = SpriteDescription(GetTiles(2, 0), -5, -4, false);
    objectSprites[Sprite_Food_Wererat] = SpriteDescription(GetTiles(3, 0), -5, -4, false);
    objectSprites[Sprite_Food_Skeleton] = SpriteDescription(GetTiles(4, 0), -5, -4, false);
    objectSprites[Sprite_Food_Fire_Wisp] = SpriteDescription(GetTiles(5, 0), -5, -4, false);
    objectSprites[Sprite_Potion_Yellow] = SpriteDescription(GetTiles(6, 1), -4, -4, false);
    objectSprites[Sprite_Potion_Orange] = SpriteDescription(GetTiles(7, 1), -4, -4, false);
    objectSprites[Sprite_Potion_Candy] = SpriteDescription(GetTiles(8, 1), -4, -4, false);
    objectSprites[Sprite_Potion_Pink] = SpriteDescription(GetTiles(9, 1), -4, -4, false);
    objectSprites[Sprite_Hp_Potion_Small] = SpriteDescription(GetTiles(0, 1), -4, -4, false);
    objectSprites[Sprite_Hp_Potion_Medium] = SpriteDescription(GetTiles(1, 1), -6, -4, false);
    objectSprites[Sprite_Hp_Potion_Big] = SpriteDescription(GetTiles(2, 1), -6, -6, false);
    objectSprites[Sprite_Mp_Potion_Small] = SpriteDescription(GetTiles(3, 1), -4, -4, false);
    objectSprites[Sprite_Mp_Potion_Medium] = SpriteDescription(GetTiles(4, 1), -6, -4, false);
    objectSprites[Sprite_Mp_Potion_Big] = SpriteDescription(GetTiles(5, 1), -6, -6, false);

    objectSprites[Sprite_Trap] = SpriteDescription(GetTiles(0, 4), 0, 0, false);
    objectSprites[Sprite_Spikes] = SpriteDescription(GetTiles(0, 3), 0, 0, false);

    objectSprites[Sprite_Stairs] = SpriteDescription(GetTiles(0, 0), 0, 0, false);

    spriteAnimationFrames[StillUnit] = AnimationDescription(stillUnitFrames, 1, 0, false, false);
    spriteAnimationFrames[Still] = AnimationDescription(stillFrames, 1, 0, false, false);
    spriteAnimationFrames[Hurt] = AnimationDescription(hurtFrames, 1, 0, true, false);
    spriteAnimationFrames[Walk] = AnimationDescription(walkFrames, 5, 5, true, true);
    spriteAnimationFrames[Idle] = AnimationDescription(idleFrames, 2, 30, true, true);
    spriteAnimationFrames[Attack] = AnimationDescription(attackFrames, 5, 8, true, false);
    spriteAnimationFrames[Special] = AnimationDescription(specialFrames, 1, 0, true, false);
    spriteAnimationFrames[Injured] = AnimationDescription(injuredFrames, 1, 20, false, false);
    spriteAnimationFrames[Sleep] = AnimationDescription(sleepFrames, 2, 42, false, true);
    spriteAnimationFrames[TrapTrigger] = AnimationDescription(triggerFrames, 14, 12, false, false);

    hardware::CopyPalette(hardware::OBJ_Palette, sprites_sheetPal);
    graphics::SetFadePaletteObjects(sprites_sheetPal);
}

void CopySpriteFrame( uint16_t spriteIdentifier, int frameNumber, int rowNumber, uint16_t targetBlock )
{
    const int TILE_SIZE_OFFSET = 32;
    const int TILE_PITCH = 52 * TILE_SIZE_OFFSET;  // 13 32x32 tiles per row

    const uint16_t* startTile = objectSprites[spriteIdentifier].tiles + ( (TILE_PITCH * rowNumber * 4)  + (frameNumber * 4 * TILE_SIZE_OFFSET) );
    // Copy the 4x4 tiles needed.

    for (int y = 0; y < 4; y++)
    {
        uint32_t* dst = (uint32_t*)hardware::Charblocks[4].GetTile8(targetBlock + 16 * y);
        uint32_t* src = (uint32_t*)( (uint16_t*)startTile + (TILE_PITCH * y));
        uint32_t* srcEnd = (uint32_t*)( (uint16_t*)startTile + (TILE_PITCH * y) + (TILE_SIZE_OFFSET * 4));
        while ( src != srcEnd)
        {
            *dst++ = *src++;
            *dst++ = *src++;
        }
    }

}
