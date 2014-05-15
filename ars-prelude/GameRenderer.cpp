#include "GameRenderer.h"
#include "gba.h"
#include <cstring>
#include "GraphicsUtil.h"
#include "GameObject.h"
#include "ObjectGraphics.h"
#include "DungeonMap.h"
#include "ui_tileset.h"
#include "obj_tileset.h"
#include "Unit.h"
#include "Item.h"
#include "tileset_dungeon.h"
#include "tileset_undersea.h"
#include "tileset_lava.h"
#include <cstdio>

GameRenderer::GameRenderer()
{

}

GameRenderer::~GameRenderer()
{

}

void GameRenderer::SetCamera( int x, int y )
{
    const int TILE_SIZE = 8;
    const int TILES_PER_ROW = 30;
    const int TILES_PER_COLUMN = 20;

    if ( x == cameraPosition.x && y == cameraPosition.y )
        return;

    cameraPosition.x = x;
    cameraPosition.y = y;

    // Perform camera bounds checking.
    if ( cameraPosition.x < 0 )
        cameraPosition.x = 0;
    if ( cameraPosition.y < 0 )
        cameraPosition.y = 0;

    if ( cameraPosition.x >= virtualScreenblockWidth * TILE_SIZE - TILES_PER_ROW * TILE_SIZE )
        cameraPosition.x = virtualScreenblockWidth * TILE_SIZE - TILES_PER_ROW * TILE_SIZE;

    if ( cameraPosition.y >= virtualScreenblockHeight * TILE_SIZE - TILES_PER_COLUMN * TILE_SIZE )
        cameraPosition.y = virtualScreenblockHeight * TILE_SIZE - TILES_PER_COLUMN * TILE_SIZE;

    redrawMap = true;
}

void GameRenderer::Init()
{
    // Allocate the virtual screenblock to a maximum size, and then fill the contents appropriately.
    this->virtualScreenblockBuffer = new short[MAX_MAP_SIZE * MAX_MAP_SIZE * 9]; // 128 x 128 map is maximum size.

    const bool ENABLE_OBJ = true;
    const bool ENABLE_BG0 = true;
    const bool ENABLE_BG1 = true;
    const bool ENABLE_BG2 = true;
    const bool ENABLE_BG3 = false;

    // We disable all the hardware drawing routines so that we can freely modify the VRAM without yielding any glitches.
    hardware::SetupDisplayControl(false, false, false, false, false);

    // Setup the correct hardware registers.
    hardware::SetupBackground(GUI_BACKGROUND, GUI_CHARBLOCK, GUI_SCREENBLOCK, false, 1);
    hardware::SetupBackground(MAP_BACKGROUND, MAP_CHARBLOCK, MAP_SCREENBLOCK, false, 2);

    LoadUnitGraphics();
    // Load default tiles.
    // Charblocks are allocated as follows:
    // CB0 = Map tiles
    // CB1 = Map tiles
    // CB2 - Gui/Text tiles
    // CB3 - Sprites
    // CB4 - Sprites

    // Screenblocks are allocated as follows:
    // SB15 - Map  screenblock
    // SB14 - Gui  screenblock
    // SB13 - Text screenblock

    mapScreenblock = &hardware::Screenblocks[MAP_SCREENBLOCK];
    guiScreenblock = &hardware::Screenblocks[GUI_SCREENBLOCK];
    textScrenblock = &hardware::Screenblocks[TEXT_SCREENBLOCK];

    const int GUI_PALETTE_OFFSET = 128;

    for (int i = 0; i < 8192; i++)
    {
        // We have to shift our 8bpp indexed pixels by GUI_PALETTE_OFFSET entries to make this work, but only if the pixel is not transparent
        const int PIXEL_MASK = (1 << 8) - 1;
        uint8_t byte1 = ui_tilesetTiles[i] & PIXEL_MASK;
        uint8_t byte2 = (ui_tilesetTiles[i] >> 8) & PIXEL_MASK;
        if ( byte1 == 0x19 ) byte1 = 0;
        if ( byte2 == 0x19 ) byte2 = 0;
        if ( byte1 != 0 ) byte1 += GUI_PALETTE_OFFSET;
        if ( byte2 != 0 ) byte2 += GUI_PALETTE_OFFSET;
        hardware::Charblocks[GUI_CHARBLOCK].data[i] = byte1 | (byte2 << 8);
        if ( i < obj_tilesetTilesLen )
            hardware::Charblocks[UNITS_CHARBLOCK].data[i] = obj_tilesetTiles[i];
        hardware::Charblocks[TEXT_CHARBLOCK].data[i] = 0;
    }

    for (int i = 0; i < 16; i++)
    {
        hardware::OBJ_Palette[16 * 15 + i] = obj_tilesetPal[i];
    }

    for (int i = GUI_PALETTE_OFFSET; i < 256; i++)
    {
        hardware::BG_Palette[i] = ui_tilesetPal[i - GUI_PALETTE_OFFSET];
    }

    // The last palette entries are reserved for text colours.
    hardware::BG_Palette[253] = RGB(31, 31, 31);
    hardware::BG_Palette[254] = RGB(8, 6, 4);

    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            guiScreenblock->SetTile(x, y, EMPTY_GUI_TILE);
            mapScreenblock->SetTile(x, y, EMPTY_MAP_TILE);
        }
    }

    textRenderer = new TextRenderer(TEXT_BACKGROUND, TEXT_CHARBLOCK, TEXT_SCREENBLOCK, 0);

    dialogCharblock = textRenderer->CreateTextArea(3, 16, 26, 4);
    hpCharblock = textRenderer->CreateTextArea(9, 0, 7, 2);
    mpCharblock = textRenderer->CreateTextArea(23, 0, 7, 2);
    expCharblock = textRenderer->CreateTextArea(3, 2, 4, 2);
    dungeonLvlCharblock = textRenderer->CreateTextArea(20, 9, 3, 1);

    // Create a map with all the different character spritesheets.
    redrawMap = true;
    isDialogVisible = false;
    ClearObjects();
    ClearText();
    // We setup the proper display control.

    hardware::SetupDisplayControl(ENABLE_OBJ, ENABLE_BG0, ENABLE_BG1, ENABLE_BG2, ENABLE_BG3);

    previousHp = previousFood = previousMp = previousMaxHp = previousMaxMp = previousExperience = -1;
    dungeonLevelTimer = -1;
    dialogRow = 0;
    gameTextTimer = 0;
    for (size_t i = 0; i < MAX_OBJECTS; i++)
    {
        previousSprites[i].targetCharblock = 0;
    }
}

void GameRenderer::Shutdown()
{
    delete textRenderer;
    delete[] virtualScreenblockBuffer;
}

void GameRenderer::LoadMap( DungeonMap& dungeonMap, DungeonSetting setting )
{
    dungeonMap.GenerateTilemap24(virtualScreenblockBuffer);
    virtualScreenblockWidth = dungeonMap.GetWidth() * 3;
    virtualScreenblockHeight = dungeonMap.GetHeight() * 3;
    redrawMap = true;

    const uint16_t* tiles = NULL;
    const uint16_t* palette = NULL;

    switch (setting)
    {
        case Castle:
            tiles = tileset_dungeonTiles;
            palette = tileset_dungeonPal;
            break;
        case Undersea:
            tiles = tileset_underseaTiles;
            palette = tileset_underseaPal;
            break;
        case Volcano:
            tiles = tileset_lavaTiles;
            palette = tileset_lavaPal;
            break;
        default:
            break;
    }

    for (int i = 0; i < 8192; i++)
    {
        hardware::Charblocks[MAP_CHARBLOCK].data[i] = tiles[i];
        if ( i < 5120) // The rest is reserved for screenblocks.
            hardware::Charblocks[MAP_CHARBLOCK + 1].data[i] = tiles[8192 + i];
    }

    for (int i = 0; i < 128; i++)
    {
        graphics::SetFadePaletteBackgroundEntry(i, palette[i]);
        hardware::BG_Palette[i] = palette[i];
    }
}

void GameRenderer::RenderDungeon()
{
    if (redrawMap == false)
        return;

    int xScroll = cameraPosition.x / 8;
    int yScroll = cameraPosition.y / 8;

    for (int x = 0; x < 32; x ++)
    {
        for (int y = 0; y < 22; y++)
        {
            uint16_t tile = virtualScreenblockBuffer[(y + yScroll) * virtualScreenblockWidth + (x + xScroll)];
            mapScreenblock->SetTile(x, y, tile);
        }
    }

    hardware::SetupBackgroundScroll(MAP_BACKGROUND, cameraPosition.x % 8, cameraPosition.y % 8);
    redrawMap = false;
}

void GameRenderer::RenderInterface(bool showDialog, int playerHp, int playerMaxHp, int playerMp, int playerMaxMp, int playerFood, int playerMaxFood, int playerExperience, int playerNeededExperience)
{
    if ( showDialog && isDialogVisible == false )
    {
        isDialogVisible = true;

        // Draw dialog box
        // Left - Right borders
        for (int y = 15; y < 20; y++)
        {
            guiScreenblock->SetTile(1, y, (y - 15 + 8) * 16 + 0);
            guiScreenblock->SetTile(2, y, (y - 15 + 8) * 16 + 1);
            guiScreenblock->SetTile(28, y, (y - 15 + 8) * 16 + 3);
            guiScreenblock->SetTile(27, y, (y - 15 + 8) * 16 + 2);
        }

        // Up - Down borders
        for (int x = 3; x < 27; x++)
        {
            guiScreenblock->SetTile(x, 15, (8) * 16 + 4);
            guiScreenblock->SetTile(x, 19, (9) * 16 + 4);
        }

        // Center
        for (int x = 3; x < 27; x++)
        {
            for (int y = 16; y < 19; y++)
                guiScreenblock->SetTile(x, y, (8) * 16 + 5);
        }
        RenderDialogue();
    }
    else if ( isDialogVisible && showDialog == false )
    {
        isDialogVisible = false;
        textRenderer->ClearTextArea(dialogCharblock, 4 * 26);
        for (int x = 0; x < 30; x++)
        {
            for (int y = 15; y < 20; y++)
                guiScreenblock->SetTile(x, y, EMPTY_GUI_TILE);
        }
    }

    if ( previousHp != playerHp || previousMaxHp != playerMaxHp )
    {
        char hpText[7];
        sprintf(hpText, "%d/%d", playerHp, playerMaxHp);

        textRenderer->ClearTextArea(hpCharblock, 14);
        textRenderer->RenderString<7>(outlineFont, hpCharblock, hpText, strlen(hpText), 1, 253, 254);

        int xHpFilled = ((float)playerHp / playerMaxHp) * 7 + 3;
        for (int x = 0; x < 9; x++)
        {
            for (int y = 0; y < 2; y++)
            {
                guiScreenblock->SetTile(x, y, GetTileID(x, x < xHpFilled ? y + 2 : y));
            }
        }
        previousHp = playerHp;
        previousMaxHp = playerMaxHp;
    }

    if ( previousMp != playerMp || previousMaxMp != playerMaxMp )
    {
        char mpText[7];
        sprintf(mpText, "%d/%d", playerMp, playerMaxMp);

        textRenderer->ClearTextArea(mpCharblock, 14);
        textRenderer->RenderString<7>(outlineFont, mpCharblock, mpText, strlen(mpText), 1, 253, 254);

        int xMpFilled = ((float)playerMp / playerMaxMp) * 7 + 3;
        for (int x = 0; x < 9; x++)
        {
            for (int y = 0; y < 2; y++)
            {
                guiScreenblock->SetTile(x + 14, y, GetTileID(x, x < xMpFilled ? y + 6 : y + 4));
            }
        }
        previousMp = playerMp;
        previousMaxMp = playerMaxMp;
    }

    if ( previousExperience != playerExperience )
    {
        char expText[3];
        sprintf(expText, "%d%%", playerExperience * 100 / playerNeededExperience);
        textRenderer->ClearTextArea(expCharblock, 8);
        textRenderer->RenderString<4>(outlineFont, expCharblock, expText, strlen(expText), 2, 253, 254);

        guiScreenblock->SetTile(0, 2, TileID<4, 10>::Value);
        guiScreenblock->SetTile(1, 2, TileID<5, 10>::Value);
        guiScreenblock->SetTile(2, 2, TileID<6, 10>::Value);
        guiScreenblock->SetTile(0, 3, TileID<4, 11>::Value);
        guiScreenblock->SetTile(1, 3, TileID<5, 11>::Value);
        guiScreenblock->SetTile(2, 3, TileID<6, 11>::Value);

        previousExperience = playerExperience;
    }

    if ( previousFood != playerFood || previousMaxFood != playerMaxFood )
    {
        int yFoodFilled = ((float)playerFood / playerMaxFood) * 5 + 2;
        // Draw food bar.
        for (int y = 0; y < 7; y++)
        {
            guiScreenblock->SetTile(28, y, GetTileID(y < yFoodFilled ? 12 : 14, y));
            guiScreenblock->SetTile(29, y, GetTileID(y < yFoodFilled ? 13 : 15, y));
        }
        previousFood = playerFood;
        previousMaxFood = playerMaxFood;
    }

    if ( dungeonLevelTimer >= 0 )
        dungeonLevelTimer++;

    if ( dungeonLevelTimer >= DUNGEON_LEVEL_TIME )
    {
        for (int x = 0; x < 13; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                guiScreenblock->SetTile(9 + x, 8 + y, EMPTY_GUI_TILE);
            }
        }
        textRenderer->ClearTextArea(dungeonLvlCharblock, 3);
        dungeonLevelTimer = -1;
    }
}

void GameRenderer::RenderObjects(GameObject* objects[MAX_OBJECTS])
{
    short currentSpriteCharblock = 32; // 0-31 are reserved charblocks
    short currentSpriteOamIndex = 8; // 0-7 reserved for damage
    short currentSpriteIndex = 0;

    const short MISC_OBJECTS_PALETTE_ID = 15;
    const short SHADOW_ID = 0;
    const short MARK_ID = 2;
    const short POISON_ID = 4;
    const short IMMOBILIZED_ID = 6;

    const int CHARACTER_WIDTH = 32;
    const int CHARACTER_HEIGHT = 32;

    int sortedIndices[MAX_OBJECTS];
    memset(sortedIndices, -1, sizeof(sortedIndices));

    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        if ( objects[i] != NULL && (objects[i]->GetType() == Object_Unit || (objects[i]->GetType() == Object_Item && ((Item*)objects[i])->IsShowForeground())) )
        {
            int currY = objects[i]->position.y;
            for (int k = 0; k < MAX_OBJECTS; k++)
            {
                if (sortedIndices[k] != -1)
                {
                    GameObject* object = objects[sortedIndices[k]];
                    if ( object->position.y < currY )
                    {
                        // Shift everything upwards.
                        for (int j = MAX_OBJECTS; j >= k; j--)
                        {
                            sortedIndices[j] = sortedIndices[j - 1];
                        }
                        sortedIndices[k] = i;
                        break;
                    }
                }
                else
                {
                    sortedIndices[k] = i;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        if ( objects[i] != NULL && objects[i]->GetType() == Object_Item && ((Item*)objects[i])->IsShowForeground() == false )
        {
            for (int k = 0; k < MAX_OBJECTS; k++)
            {
                if (sortedIndices[k] == -1)
                {
                    sortedIndices[k] = i;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < 96; i++)
    {
        SetObject(i, ATTR0_HIDE, 0, 0);
    }

    if ( gameTextTimer < GAME_TEXT_DISPLAY_TIME )
    {
        static const int TEXT_WIDTH = 32;
        static const int TEXT_HEIGHT = 8;
        gameTextTimer++;
        Point16 gameTextScreenPosition;
        gameTextScreenPosition.x = gameTextPosition.x - cameraPosition.x;
        gameTextScreenPosition.y = gameTextPosition.y - cameraPosition.y;
        // Perform screen clipping.
        if ( gameTextScreenPosition.x >= -TEXT_WIDTH &&
                gameTextScreenPosition.x < SCREEN_WIDTH &&
                gameTextScreenPosition.y >= -TEXT_HEIGHT &&
                gameTextScreenPosition.y < SCREEN_HEIGHT )
        {
            for (int i = 0; i < 5; i++)
            {
                if ( gameTextSprites[i] == -1 )
                    break;
                hardware::SetSprite4(i, gameTextScreenPosition.x + gameTextSpriteOffsets[i], gameTextScreenPosition.y,
                                     gameTextSprites[i], MISC_OBJECTS_PALETTE_ID, 0, 0, false, false, 2);
            }
        }
        if ( gameTextTimer % 4 == 0 )
        {
            if ( gameTextTimer < 15 )
                gameTextPosition.y -= 2;
            else
                gameTextPosition.y += 1;
        }
        if ( gameTextTimer == GAME_TEXT_DISPLAY_TIME )
        {
            for (int i = 0; i < 5; i++)
                hardware::HideSprite(i);
        }
    }

    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        if ( sortedIndices[i] != -1 )
        {
            GameObject* object = objects[sortedIndices[i]];

            SpriteDescription& spriteGraphics = objectSprites[object->GetSpriteIdentifier()];
            // Render character.
            Point16 characterScreenPosition;
            characterScreenPosition.x = object->position.x - cameraPosition.x + spriteGraphics.xOffset;
            characterScreenPosition.y = object->position.y - cameraPosition.y + spriteGraphics.yOffset;
            // Perform screen clipping.
            if ( characterScreenPosition.x >= -CHARACTER_WIDTH &&
                    characterScreenPosition.x < SCREEN_WIDTH &&
                    characterScreenPosition.y >= -CHARACTER_HEIGHT &&
                    characterScreenPosition.y < SCREEN_HEIGHT )
            {
                Point16 shadowPosition = characterScreenPosition;
                shadowPosition.x += 9;
                shadowPosition.y += 28;

                // Adjust negative out of screen position
                if ( characterScreenPosition.x < 0 )
                    characterScreenPosition.x = 512 + characterScreenPosition.x;
                if ( characterScreenPosition.y < 0 )
                    characterScreenPosition.y = 256 + characterScreenPosition.y;

                if ( shadowPosition.x < 0 )
                    shadowPosition.x = 512 + shadowPosition.x;
                if ( shadowPosition.y < 0 )
                    shadowPosition.y = 256 + shadowPosition.y;

                bool flipHorizontal = false;

                if ( object->currentDirection & East )
                    flipHorizontal = true;

                if ( object->GetType() == Object_Unit)
                {
                    Unit* unit = (Unit*)object;
                    if ( unit->IsHumanControlled() && unit->HasTrapNearby() )
                    {
                        // Render exclamation mark.
                        hardware::SetSprite4(currentSpriteOamIndex++, characterScreenPosition.x + 8, characterScreenPosition.y - 8,
                                             MARK_ID, MISC_OBJECTS_PALETTE_ID, 0, 1, false, false, 2);
                    }
                    if ( unit->IsPoisoned() )
                    {
                        // Render poisoned icon.
                        hardware::SetSprite4(currentSpriteOamIndex++, characterScreenPosition.x, characterScreenPosition.y,
                                             POISON_ID, MISC_OBJECTS_PALETTE_ID, 0, 1, false, false, 2);
                    }
                    if ( unit->IsImmobilized() )
                    {
                        // Render immobilized icon.
                        hardware::SetSprite4(currentSpriteOamIndex++, characterScreenPosition.x + 16, characterScreenPosition.y,
                                             IMMOBILIZED_ID, MISC_OBJECTS_PALETTE_ID, 0, 1, false, false, 2);
                    }
                }

                // Render character.
                hardware::SetSprite8(currentSpriteOamIndex++, characterScreenPosition.x, characterScreenPosition.y,
                                     currentSpriteCharblock, 0, 2, flipHorizontal, false, 2);

                if ( spriteGraphics.renderShadow )
                {
                    // Render character shadow.
                    hardware::SetSprite4(currentSpriteOamIndex++, shadowPosition.x, shadowPosition.y,
                                         SHADOW_ID, MISC_OBJECTS_PALETTE_ID, 0, 1, false, false, 2);
                }

                int yOffset = 0;
                if ( object->SupportsDirection() )
                {
                    if ( object->currentDirection & East || object->currentDirection & West)
                        yOffset = 1;
                    else if ( object->currentDirection & North )
                        yOffset = 2;
                }

                sprites[currentSpriteIndex].spriteIdentifier = object->GetSpriteIdentifier();
                sprites[currentSpriteIndex].frameNumber = object->GetCurrentFrame();
                sprites[currentSpriteIndex].rowNumber = yOffset;
                sprites[currentSpriteIndex].targetCharblock = currentSpriteCharblock;

                currentSpriteCharblock += 4;
                if ( currentSpriteCharblock % 16 == 0 ) // Since we're using 2D mapping we need to skip two lines everytime we reach the end or we'll overwrite a part of our sprites.
                    currentSpriteCharblock += 16 * 3;
                currentSpriteIndex++;
            }
        }
    }

    UpdateObjects();

    hardware::Charblock* spritesCharblock = &hardware::Charblocks[UNITS_CHARBLOCK];

    // Copy the information in the right charblock
    for (short i = 0; i < currentSpriteIndex; i++)
    {
        if ( sprites[i] == previousSprites[i] )
            continue;

        SpriteDescription& spriteGraphics = objectSprites[sprites[i].spriteIdentifier];

        CopySpriteFrame(sprites[i].spriteIdentifier, sprites[i].frameNumber, sprites[i].rowNumber, sprites[i].targetCharblock);

        previousSprites[i] = sprites[i];
    }

}

void GameRenderer::DisplayText( char* text, int textLength)
{
    if ( dialogRow <= 1 )
    {
        memcpy(textboxText[dialogRow], text, textLength);
        textboxText[dialogRow][textLength] = '\0';
        dialogRow++;
    }
    else
    {
        memcpy(textboxText[0], textboxText[1], 32);
        memcpy(textboxText[1], textboxText[2], 32);
        memcpy(textboxText[2], text, textLength);
        textboxText[2][textLength] = '\0';
    }
    RenderDialogue();
}

void GameRenderer::DisplayText( char* text )
{
    DisplayText(text, strlen(text));
}

void GameRenderer::ClearText()
{
    dialogRow = 0;
    for (int i = 0; i < 32; i++)
    {
        textboxText[0][i] = '\0';
        textboxText[1][i] = '\0';
        textboxText[2][i] = '\0';
    }
    textRenderer->ClearTextArea(dialogCharblock, 4 * 26);
}

void GameRenderer::RenderDialogue()
{
    if ( isDialogVisible )
    {
        textRenderer->ClearTextArea(dialogCharblock, 4 * 26);
        textRenderer->RenderString<26>(normalFont, dialogCharblock, textboxText[0], strlen(textboxText[0]), 0, 254, 253);
        textRenderer->RenderString<26>(normalFont, dialogCharblock, textboxText[1], strlen(textboxText[1]), 1 + 8, 254, 253);
        textRenderer->RenderString<26>(normalFont, dialogCharblock, textboxText[2], strlen(textboxText[2]), 2 + 16, 254, 253);
    }
}

void GameRenderer::ShowDungeonLevel( int level )
{
    dungeonLevelTimer = 0;

    for (int x = 0; x < 13; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            guiScreenblock->SetTile(9 + x, 8 + y, GetTileID(x, y + 13));
        }
    }
    char lvlText[3];
    sprintf(lvlText, "%d", level);
    textRenderer->RenderString<3>(normalFont, dungeonLvlCharblock, lvlText, strlen(lvlText), 0, 254, 253);
}

void GameRenderer::DisplayDamage( Point16 position, int amount )
{
    const short ZERO_ID = 8;
    gameTextTimer = 0;
    gameTextPosition = position;
    char dmgTxt[5];
    sprintf(dmgTxt, "%d", amount);
    int len = strlen(dmgTxt);
    gameTextPosition.x += (3 - len) * 3;
    for (int i = 0; i < 5; i++)
        gameTextSprites[i] = -1;
    for (int i = 0; i < len; i++)
    {
        int chOffset = dmgTxt[i] - '0';
        gameTextSprites[i] = ZERO_ID + chOffset;
        if ( i > 0 )
            gameTextSpriteOffsets[i] = normalFont.glyphs[dmgTxt[i]].xAdvance + 1;
    }
}

void GameRenderer::DisplayLevelUp( Point16 position )
{
    gameTextTimer = 0;
    gameTextPosition = position;
    const short LEVEL_UP_ID = 40;
    for (int i = 0; i < 5; i++)
    {
        gameTextSprites[i] = LEVEL_UP_ID + i;
        gameTextSpriteOffsets[i] = i * 8;
    }
}
