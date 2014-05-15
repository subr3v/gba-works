#include "CutsceneScene.h"
#include "SceneManager.h"
#include "stdint.h"
#include "ObjectGraphics.h"
#include "gba.h"
#include "System.h"
#include "GraphicsUtil.h"
#include "intro_tileset.h"
#include "intro_tileset_fire.h"
#include "intro_tileset_crystal.h"
#include "GameFonts.h"
#include "GameWorld.h"
#include "GameObjectFactory.h"
#include "Unit.h"
#include <stdlib.h>

const uint16_t dungeonEntranceMap[30 * 20] =
{
    97, 98, 99, 49, 50, 51, 151, 152, 153, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 151, 152, 153, 49, 50, 51, 148, 149, 150,
    113, 114, 115, 65, 66, 67, 167, 168, 169, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 167, 168, 169, 65, 66, 67, 164, 165, 166,
    129, 130, 131, 81, 82, 83, 183, 184, 185, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 183, 184, 185, 81, 82, 83, 180, 181, 182,
    97, 98, 99, 49, 50, 51, 7, 8, 9, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 10, 11, 12, 49, 50, 51, 148, 149, 150,
    113, 114, 115, 65, 66, 67, 23, 24, 25, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 26, 27, 28, 65, 66, 67, 164, 165, 166,
    129, 130, 131, 81, 82, 83, 39, 40, 41, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 42, 43, 44, 81, 82, 83, 148, 149, 150,
    97, 98, 99, 49, 50, 51, 55, 56, 57, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 58, 59, 60, 49, 50, 51, 164, 165, 166,
    113, 114, 115, 65, 66, 67, 71, 72, 73, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 74, 75, 76, 65, 66, 67, 180, 181, 182,
    129, 130, 131, 81, 82, 83, 87, 88, 89, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 90, 91, 92, 81, 82, 83, 148, 149, 150,
    97, 98, 99, 49, 50, 51, 103, 104, 105, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 106, 107, 108, 49, 50, 51, 164, 165, 166,
    113, 114, 115, 65, 66, 67, 119, 120, 121, 17, 18, 19, 17, 18, 19, 17, 18, 19, 17, 18, 19, 122, 123, 124, 65, 66, 67, 180, 181, 182,
    129, 130, 131, 81, 82, 83, 135, 136, 137, 33, 34, 35, 33, 34, 35, 33, 34, 35, 33, 34, 35, 138, 139, 140, 81, 82, 83, 148, 149, 150,
    97, 98, 99, 49, 50, 51, 49, 50, 51, 55, 56, 57, 1, 2, 3, 1, 2, 3, 58, 59, 60, 49, 50, 51, 49, 50, 51, 164, 165, 166,
    113, 114, 115, 65, 66, 67, 65, 66, 67, 71, 72, 73, 17, 18, 19, 17, 18, 19, 74, 75, 76, 65, 66, 67, 65, 66, 67, 180, 181, 182,
    129, 130, 131, 81, 82, 83, 81, 82, 83, 87, 88, 89, 33, 34, 35, 33, 34, 35, 90, 91, 92, 81, 82, 83, 81, 82, 83, 148, 149, 150,
    52, 53, 54, 100, 101, 102, 49, 50, 51, 55, 56, 57, 1, 2, 3, 1, 2, 3, 58, 59, 60, 49, 50, 51, 193, 194, 195, 196, 197, 198,
    68, 69, 70, 116, 117, 118, 65, 66, 67, 71, 72, 73, 17, 18, 19, 17, 18, 19, 74, 75, 76, 65, 66, 67, 209, 210, 211, 212, 213, 214,
    84, 85, 86, 132, 133, 134, 81, 82, 83, 87, 88, 89, 33, 34, 35, 33, 34, 35, 90, 91, 92, 81, 82, 83, 225, 226, 227, 228, 229, 230,
    163, 163, 163, 97, 98, 99, 49, 50, 51, 55, 56, 57, 1, 2, 3, 1, 2, 3, 58, 59, 60, 49, 50, 51, 164, 165, 166, 163, 163, 163,
    163, 163, 163, 113, 114, 115, 65, 66, 67, 71, 72, 73, 17, 18, 19, 17, 18, 19, 74, 75, 76, 65, 66, 67, 180, 181, 182, 163, 163, 163
};


const uint16_t dungeonCampfireMap[30 * 20] =
{
    97, 98, 99, 49, 50, 51, 151, 152, 153, 151, 152, 153, 244, 245, 246, 244, 245, 246, 153, 152, 153, 151, 152, 153, 49, 50, 51, 148, 149, 150,
    113, 114, 115, 65, 66, 67, 167, 168, 169, 167, 168, 169, 17, 17, 18, 17, 18, 19, 169, 168, 169, 167, 168, 169, 65, 66, 67, 164, 165, 166,
    129, 130, 131, 81, 82, 83, 183, 184, 185, 183, 184, 185, 33, 33, 34, 33, 34, 35, 185, 184, 185, 183, 184, 185, 81, 82, 83, 180, 181, 182,
    97, 98, 99, 49, 50, 51, 7, 8, 9, 241, 242, 243, 1, 2, 3, 1, 2, 3, 250, 251, 252, 10, 11, 12, 49, 50, 51, 148, 149, 150,
    113, 114, 115, 65, 66, 67, 23, 24, 25, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 26, 27, 28, 65, 66, 67, 164, 165, 166,
    129, 130, 131, 81, 82, 83, 39, 40, 41, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 42, 43, 44, 81, 82, 83, 148, 149, 150,
    97, 98, 99, 49, 50, 51, 55, 56, 57, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 58, 59, 60, 49, 50, 51, 164, 165, 166,
    113, 114, 115, 65, 66, 67, 71, 72, 73, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 74, 75, 76, 65, 66, 67, 180, 181, 182,
    129, 130, 131, 81, 82, 83, 87, 88, 89, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 90, 91, 92, 81, 82, 83, 148, 149, 150,
    97, 98, 99, 49, 50, 51, 103, 104, 105, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 106, 107, 108, 49, 50, 51, 164, 165, 166,
    113, 114, 115, 65, 66, 67, 119, 120, 121, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 122, 123, 124, 65, 66, 67, 180, 181, 182,
    129, 130, 131, 81, 82, 83, 135, 136, 137, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 138, 139, 140, 81, 82, 83, 148, 149, 150,
    97, 98, 99, 49, 50, 51, 49, 50, 51, 55, 56, 57, 1, 2, 3, 1, 2, 3, 58, 59, 60, 49, 50, 51, 49, 50, 51, 164, 165, 166,
    113, 114, 115, 65, 66, 67, 65, 66, 67, 71, 72, 73, 17, 18, 19, 17, 18, 19, 74, 75, 76, 65, 66, 67, 65, 66, 67, 180, 181, 182,
    129, 130, 131, 81, 82, 83, 81, 82, 83, 87, 88, 89, 33, 34, 35, 33, 34, 35, 90, 91, 92, 81, 82, 83, 81, 82, 83, 148, 149, 150,
    52, 53, 54, 100, 101, 102, 49, 50, 51, 55, 56, 57, 1, 2, 3, 1, 2, 3, 58, 59, 60, 49, 50, 51, 193, 194, 195, 196, 197, 198,
    68, 69, 70, 116, 117, 118, 65, 66, 67, 71, 72, 73, 17, 18, 19, 17, 18, 19, 74, 75, 76, 65, 66, 67, 209, 210, 211, 212, 213, 214,
    84, 85, 86, 132, 133, 134, 81, 82, 83, 87, 88, 89, 33, 34, 35, 33, 34, 35, 90, 91, 92, 81, 82, 83, 225, 226, 227, 228, 229, 230,
    163, 163, 163, 97, 98, 99, 49, 50, 51, 55, 56, 57, 1, 2, 3, 1, 2, 3, 58, 59, 60, 49, 50, 51, 164, 165, 166, 163, 163, 163,
    163, 163, 163, 113, 114, 115, 65, 66, 67, 71, 72, 73, 17, 18, 19, 17, 18, 19, 74, 75, 76, 65, 66, 67, 180, 181, 182, 163, 163, 163
};

const uint16_t dungeonEndingMap[30 * 20] =
{
    97, 98, 99, 49, 50, 51, 151, 152, 153, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 151, 152, 153, 49, 50, 51, 148, 149, 150,
    113, 114, 115, 65, 66, 67, 167, 168, 169, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 167, 168, 169, 65, 66, 67, 164, 165, 166,
    129, 130, 131, 81, 82, 83, 183, 184, 185, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 183, 184, 185, 81, 82, 83, 180, 181, 182,
    97, 98, 99, 49, 50, 51, 7, 8, 9, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 10, 11, 12, 49, 50, 51, 148, 149, 150,
    113, 114, 115, 65, 66, 67, 23, 24, 25, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 26, 27, 28, 65, 66, 67, 164, 165, 166,
    129, 130, 131, 81, 82, 83, 39, 40, 41, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 42, 43, 44, 81, 82, 83, 148, 149, 150,
    97, 98, 99, 49, 50, 51, 55, 56, 57, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 58, 59, 60, 49, 50, 51, 164, 165, 166,
    113, 114, 115, 65, 66, 67, 71, 72, 73, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 74, 75, 76, 65, 66, 67, 180, 181, 182,
    129, 130, 131, 81, 82, 83, 87, 88, 89, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 90, 91, 92, 81, 82, 83, 148, 149, 150,
    97, 98, 99, 49, 50, 51, 103, 104, 105, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 106, 107, 108, 49, 50, 51, 164, 165, 166,
    113, 114, 115, 65, 66, 67, 119, 120, 121, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 122, 123, 124, 65, 66, 67, 180, 181, 182,
    129, 130, 131, 81, 82, 83, 135, 136, 137, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 138, 139, 140, 81, 82, 83, 148, 149, 150,
    97, 98, 99, 49, 50, 51, 49, 50, 51, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 49, 50, 51, 49, 50, 51, 164, 165, 166,
    113, 114, 115, 65, 66, 67, 65, 66, 67, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 65, 66, 67, 65, 66, 67, 180, 181, 182,
    129, 130, 131, 81, 82, 83, 81, 82, 83, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 81, 82, 83, 81, 82, 83, 148, 149, 150,
    52, 53, 54, 100, 101, 102, 49, 50, 51, 13, 14, 15, 61, 62, 63, 109, 110, 111, 157, 158, 159, 49, 50, 51, 193, 194, 195, 196, 197, 198,
    68, 69, 70, 116, 117, 118, 65, 66, 67, 29, 30, 31, 77, 78, 79, 125, 126, 127, 173, 174, 175, 65, 66, 67, 209, 210, 211, 212, 213, 214,
    84, 85, 86, 132, 133, 134, 81, 82, 83, 45, 46, 47, 93, 94, 95, 141, 142, 143, 189, 190, 191, 81, 82, 83, 225, 226, 227, 228, 229, 230,
    163, 163, 163, 97, 98, 99, 49, 50, 51, 55, 56, 57, 1, 2, 3, 1, 2, 3, 58, 59, 60, 49, 50, 51, 164, 165, 166, 163, 163, 163,
    163, 163, 163, 113, 114, 115, 65, 66, 67, 71, 72, 73, 17, 18, 19, 17, 18, 19, 74, 75, 76, 65, 66, 67, 180, 181, 182, 163, 163, 163
};

const uint16_t* cutsceneMaps[3] =
{
    dungeonEntranceMap,
    dungeonCampfireMap,
    dungeonEndingMap,
};

const DialoguePart part0[6] =
{
    DialoguePart(UnitType_Mage, "According to my book", "this is the entrance of ", "the ancient crypt"),
    DialoguePart(UnitType_Thief, "You're always fiddling", "with those books!", ""),
    DialoguePart(UnitType_Mage, "...", " \"thanks\" would be enough", ""),
    DialoguePart(UnitType_Knight, "You two stop!", "We need to be careful", "this place is full of dangers"),
    DialoguePart(UnitType_Mage, "Let's check around", "those earthquakes", "are scaring me"),
    DialoguePart(UnitType_Knight, "Move on!", "We must hurry", "")
};

const DialoguePart part1[5] =
{
    DialoguePart(UnitType_Mage, "These are no ordinary creatures:", "some magical power lies", "inside the dungeon"),
    DialoguePart(UnitType_Mage, "It seems like those", "creatures were brought back", "to life with magic"),
    DialoguePart(UnitType_Thief, "That certainly explains", "why they disappear after", "being killed..."),
    DialoguePart(UnitType_Knight, "We need to", "investigate this further", ""),
    DialoguePart(UnitType_Mage, "Ok, we've had enough rest.", "Let's go", "")
};

const DialoguePart part2[6] =
{
    DialoguePart(UnitType_Knight, "I just hope this", "trip will be worth it", ""),
    DialoguePart(UnitType_Knight, "We really need to", "raise funds for the", "next dragon tournament"),
    DialoguePart(UnitType_Mage, "This book never lied", "to me, I'm pretty sure", "we'll find something!"),
    DialoguePart(UnitType_Mage, "...And then the marauders", "will try to steal", "our treasures as always"),
    DialoguePart(UnitType_Knight, "Better get ready", "for that...", ""),
    DialoguePart(UnitType_Mage, "I'm getting nervous", "about this, move on!", "")
};

const DialoguePart part3[5] =
{
    DialoguePart(UnitType_Knight, "This dungeon can't be", "that deep, we are", "reaching the end of it"),
    DialoguePart(UnitType_Mage, "I can sense a magical", "power growing in strength", "as we go deeper..."),
    DialoguePart(UnitType_Thief, "Monsters are getting", "stronger as well! ", ""),
    DialoguePart(UnitType_Thief, "One doesn't get to slay", "wyverns every day!", ""),
    DialoguePart(UnitType_Mage, "We must keep going,", "the treasure lies ahead.", "")
};

const DialoguePart part4[9] =
{
    DialoguePart(UnitType_Thief, "Never seen a crystal like", "this, must be worth a fortune!", ""),
    DialoguePart(UnitType_Knight, "So this is the secret", "that lies within", "the crypt...but."),
    DialoguePart(UnitType_Knight, "Hey there is", "someone inside!", ""),
    DialoguePart(UnitType_Mage, "I can vaguely identify", "the crest on the mantle, ", "a Moonbear?"),
    DialoguePart(UnitType_Knight, "Those were part of ", "the royal guard before", "the war I recall..."),
    DialoguePart(UnitType_Mage, "What should we do with,", "this man now? He seems", "to be...still alive"),
    DialoguePart(UnitType_Thief, "The crystal should be", "brought to our contractor", ""),
    DialoguePart(UnitType_Mage, "", "	...", ""),
    DialoguePart(UnitType_Mage, "Looks like it's moving!", "", "")
};

const CutsceneDialog cutsceneDialogues[5] =
{
    CutsceneDialog(Cutscene_Intro, part0, sizeof(part0) / sizeof(DialoguePart)),
    CutsceneDialog(Cutscene_Campfire, part1, sizeof(part1) / sizeof(DialoguePart)),
    CutsceneDialog(Cutscene_Campfire, part2, sizeof(part2) / sizeof(DialoguePart)),
    CutsceneDialog(Cutscene_Campfire, part3, sizeof(part3) / sizeof(DialoguePart)),
    CutsceneDialog(Cutscene_Crystal, part4, sizeof(part4) / sizeof(DialoguePart)),
};

CutsceneScene::CutsceneScene( int cutsceneNumber, Scene* nextScene )
{
    currentDialog = cutsceneDialogues[cutsceneNumber];
    currentMap = currentDialog.cutsceneMap;
    this->nextScene = nextScene;
}

void CutsceneScene::OnEnter()
{
    gameRenderer.Init();
    hardware::SetupDisplayControl(false, false, false, false, false);
    ClearObjects();

    const uint16_t* palette;
    const uint16_t* tiledata;
    const uint16_t* mapData;

    switch (currentMap)
    {
        case Cutscene_Intro:
            palette = intro_tilesetPal;
            mapData = dungeonEntranceMap;
            tiledata = intro_tilesetTiles;
            break;
        case Cutscene_Campfire:
            palette = intro_tileset_firePal;
            mapData = dungeonCampfireMap;
            tiledata = intro_tileset_fireTiles;
            break;
        case Cutscene_Crystal:
            palette = intro_tileset_crystalPal;
            mapData = dungeonEndingMap;
            tiledata = intro_tileset_crystalTiles;
            break;
        default:
            break;
    }

    graphics::SetFadePaletteBackground(palette);
    hardware::LoadCharblock(0, tiledata);
    hardware::LoadCharblock(1, tiledata + 8192);

    hardware::SetupBackground(0, 0, 31, false, 2);
    textRenderer = new TextRenderer(1, 2, 30, 0);

    for (int x = 0; x < 30; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            hardware::Screenblocks[31].SetTile(x, y, mapData[y * 30 + x] - 1);
        }
    }

    hardware::SetupDisplayControl(true, true, true, false, false);
    graphics::FadeInBackground(1);

    world = new GameWorld(NULL);

    mage = GameObjectFactory::CreateUnit(NULL, UnitType_Mage);
    thief = GameObjectFactory::CreateUnit(NULL, UnitType_Thief);
    knight = GameObjectFactory::CreateUnit(NULL, UnitType_Knight);
    world->AddObject(mage);
    world->AddObject(thief);
    world->AddObject(knight);

    mage->position = Point16(94, 0);
    thief->position = Point16(110, 0);
    knight->position = Point16(126, 0);

    mage->currentDirection = knight->currentDirection = thief->currentDirection = South;

    timer = 0;
    dialogLines[0] = dialogLines[1] = dialogLines[2] = NULL;
    currentCharacter = 0;
    textArea = NULL;
    currentPart = 0;
    ChangeState(State_UnitsWalkingIn);
}

void CutsceneScene::OnExit()
{
    gameRenderer.Shutdown();
}

void CutsceneScene::Update()
{
    world->Update();

    timer++;
    int currentY;
    float movementPercent;

    switch (currentState)
    {
        case State_UnitsWalkingIn:
        case State_UnitsWalkingOut:
            moveTimer++;
            movementPercent = (float)moveTimer / (float)MOVEMENT_TIME;
            if ( movementPercent >= 1.0f )
            {
                movementPercent = 1.0f;
                ChangeState(movementNextState);
            }
            currentY = moveStartY + (moveEndY - moveStartY) * movementPercent;
            SetUnitsPosition(currentY);
            break;
        case State_UnitsTalking:
            if ( currentLine != 3 && hardware::IsButtonPressed(KEY_A))
            {
                currentLine = 3;
            }
            else if ( currentLine != 3 )
            {
                if ( timer >= 2 )
                {
                    timer = 0;
                    currentCharacter++;
                    if ( currentLine < 3 && currentCharacter >= strlen(dialogLines[currentLine]))
                    {
                        currentCharacter = 0;
                        currentLine++;
                    }
                }
            }
            else
            {
                if ( hardware::IsButtonPressed(KEY_A))
                {
                    if ( currentPart < currentDialog.partsCount )
                    {
                        ShowText(currentDialog.parts[currentPart].unit, (const char**)currentDialog.parts[currentPart].dialogLines);
                        currentPart++;
                    }
                    else
                    {
                        textRenderer->ResetAreas();
                        ChangeState(State_UnitsWalkingOut);
                        mage->currentDirection = knight->currentDirection = thief->currentDirection = South;
                        mage->DisplayAnimation(Walk);
                        knight->DisplayAnimation(Walk);
                        thief->DisplayAnimation(Walk);
                    }
                }
                else if ( timer >= 30 )
                {
                    timer = 0;
                    // Display "press continue button"
                }
            }
            break;
        default:
            break;
    }
}

void CutsceneScene::Render()
{
    gameRenderer.RenderObjects(world->objects);
    RenderText();
}

void CutsceneScene::ShowText( UnitType talkingUnit, const char** dialogLines )
{
    textRenderer->ResetAreas();

    mage->DisplayAnimation(StillUnit);
    thief->DisplayAnimation(StillUnit);
    knight->DisplayAnimation(StillUnit);

    switch (talkingUnit)
    {
        case UnitType_Mage:
            textArea = textRenderer->CreateTextArea(6, 4, 20, 4);
            mage->DisplayAnimation(Idle);
            break;
        case UnitType_Knight:
            textArea = textRenderer->CreateTextArea(14, 4, 20, 4);
            knight->DisplayAnimation(Idle);
            break;
        case UnitType_Thief:
            textArea = textRenderer->CreateTextArea(10, 4, 20, 4);
            thief->DisplayAnimation(Idle);
            break;
        default:
            break;
    }

    this->currentCharacter = 0;
    this->currentLine = 0;
    for (int i = 0; i < 3; i++)
        this->dialogLines[i] = dialogLines[i];
}

void CutsceneScene::RenderText()
{
    if ( textArea == NULL )
        return;

    for (int i = 0; i < currentLine; i++)
    {
        textRenderer->RenderString<20>(normalFont, textArea, dialogLines[i], strlen(dialogLines[i]), i * 10, 254, 253);
    }
    if ( currentLine != 3 )
    {
        textRenderer->RenderString<20>(normalFont, textArea, dialogLines[currentLine], currentCharacter, currentLine * 10, 254, 253);
    }
}

void CutsceneScene::ChangeState( CutsceneState newState )
{
    if ( newState == State_UnitsWalkingIn || newState == State_UnitsWalkingOut )
    {
        mage->DisplayAnimation(Walk);
        thief->DisplayAnimation(Walk);
        knight->DisplayAnimation(Walk);
        moveTimer = 0;
    }

    if ( newState == State_UnitsWalkingIn )
    {
        movementNextState = State_UnitsTalking;
        switch (currentMap)
        {
            case Cutscene_Intro:
                talkingDirection = South;
                moveStartY = 16;
                moveEndY = 74;
                break;
            case Cutscene_Campfire:
                talkingDirection = South;
                moveStartY = moveEndY = 74;
                moveTimer = MOVEMENT_TIME;
                break;
            case Cutscene_Crystal:
                talkingDirection = North;
                moveStartY = moveEndY = 124;
                moveTimer = MOVEMENT_TIME;
                break;
            default:
                break;
        }

    }

    if ( newState == State_UnitsWalkingOut )
    {
        switch (currentMap)
        {
            case Cutscene_Campfire:
            case Cutscene_Intro:
                moveStartY = 74;
                moveEndY = 160;
                break;
            case Cutscene_Crystal:
                moveStartY = 124;
                moveEndY = 160;
                break;
            default:
                break;
        }
        movementNextState = State_End;
    }

    if ( newState == State_UnitsTalking )
    {
        mage->DisplayAnimation(Idle);
        knight->DisplayAnimation(Idle);
        thief->DisplayAnimation(Idle);

        thief->currentDirection = talkingDirection;
        mage->currentDirection = talkingDirection;
        knight->currentDirection = talkingDirection;
        ShowText(currentDialog.parts[currentPart].unit, (const char**)currentDialog.parts[currentPart].dialogLines);
        currentPart++;
    }

    if ( newState == State_End )
    {
        graphics::FadeOutObjects(1);
        graphics::FadeOutBackground(1);
        GetSceneManager()->SetScene(nextScene, true);

    }

    currentState = newState;
}

void CutsceneScene::SetUnitsPosition( int yPosition )
{
    thief->position.y = yPosition;
    mage->position.y = knight->position.y = yPosition - 4;
}
