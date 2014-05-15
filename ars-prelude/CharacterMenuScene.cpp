#include "CharacterMenuScene.h"

#include "character_menu_obj_sheet.h"
#include "character_menu_bg_sheet.h"

#include "System.h"
#include "GameFonts.h"

#include "gba.h"

#include <cstring>
#include "GameScene.h"
#include "SceneManager.h"
#include "GraphicsUtil.h"

const uint16_t lockedCharacterMap[30 * 20] =
{
    1, 2, 2, 2, 2, 2, 2, 2, 2, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 2, 2, 2, 2, 2, 2, 4,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 18, 18, 20,
    17, 18, 18, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 18, 18, 20,
    17, 18, 18, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 18, 18, 20,
    17, 18, 18, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 18, 18, 20,
    17, 18, 18, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 18, 18, 20,
    17, 18, 18, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 8, 20,
    17, 18, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 24, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    25, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 11, 12,
    19, 257, 258, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 260, 261, 19,
    19, 273, 274, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 276, 277, 19,
    19, 289, 290, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 292, 293, 19,
    19, 305, 306, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 308, 309, 19,
    19, 321, 322, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 324, 325, 19
};

const uint16_t unlockedCharacterMap[30 * 20] =
{
    1, 2, 2, 2, 2, 2, 2, 2, 2, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 2, 2, 2, 2, 2, 2, 4,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 161, 162, 163, 164, 165, 166, 263, 264, 265, 266, 267, 268, 18, 18, 20,
    17, 18, 18, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 177, 178, 179, 180, 181, 182, 279, 280, 281, 282, 283, 284, 18, 18, 20,
    17, 18, 18, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 193, 194, 195, 196, 197, 198, 295, 296, 297, 298, 299, 300, 18, 18, 20,
    17, 18, 18, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 209, 210, 211, 212, 213, 214, 311, 312, 313, 314, 315, 316, 18, 18, 20,
    17, 18, 18, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 225, 226, 227, 228, 229, 230, 327, 328, 329, 330, 331, 332, 18, 18, 20,
    17, 18, 18, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 241, 242, 243, 244, 245, 246, 343, 344, 345, 346, 347, 348, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 8, 20,
    17, 18, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 24, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20,
    25, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 11, 12,
    19, 257, 258, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 260, 261, 19,
    19, 273, 274, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 276, 277, 19,
    19, 289, 290, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 292, 293, 19,
    19, 305, 306, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 308, 309, 19,
    19, 321, 322, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 324, 325, 19
};

CharacterMenuScene::CharacterMenuScene()
{
    jobs[0] = JobInformation(UnitType_Mage, "Mage",
                             "Mage is a strong but fragile character.",
                             "Skills: Fireball, Summon Slime, Root",
                             "Recommended for skilled players.", 32, 36, 40);
    jobs[1] = JobInformation(UnitType_Knight, "Knight",
                             "Knight is a very resistant character.",
                             "Skills: Push, Spiral, Double Attack",
                             "Easy to use and good for starters.", 32, 36, 40);
    jobs[2] = JobInformation(UnitType_Thief, "Thief",
                             "Thief is a very versatile character.",
                             "Skills: Teleport, Reveal, Poison Dart",
                             "Reccommended for everyone.", 32, 36, 40);
    jobs[3] = JobInformation(UnitType_Morpher, "Morpher",
                             "Thief is a very versatile character.",
                             "Skills: Teleport, Reveal, Poison Dart",
                             "Reccommended for everyone.", 32, 36, 40);
    isSecretJobUnlocked = false;
}

void CharacterMenuScene::OnEnter()
{
    currentJob = 0;

    const bool ENABLE_OBJ = true;
    const bool ENABLE_BG0 = true;
    const bool ENABLE_BG1 = true;
    const bool ENABLE_BG2 = false;
    const bool ENABLE_BG3 = false;

    // Setup the correct hardware registers.
    hardware::SetupDisplayControl(false, false, false, false, false);
    ClearObjects();

    graphics::SetFadePaletteBackground(character_menu_bg_sheetPal);
    graphics::SetFadePaletteObjects(character_menu_obj_sheetPal);

    // The last palette entries are reserved for text colours.
    graphics::SetFadePaletteBackgroundEntry(253, RGB(31, 31, 31));
    graphics::SetFadePaletteBackgroundEntry(254, RGB(8, 6, 4));
    graphics::SetFadePaletteBackgroundEntry(255, RGB(20, 16, 11));

    hardware::LoadCharblock(0, character_menu_bg_sheetTiles);
    hardware::LoadCharblock(1, character_menu_bg_sheetTiles + 8192);
    hardware::LoadCharblock(4, character_menu_obj_sheetTiles);

    hardware::Screenblock* screenblock = &hardware::Screenblocks[31];

    const uint16_t* screenmap = isSecretJobUnlocked ? unlockedCharacterMap : lockedCharacterMap;

    for (int x = 0; x < 30; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            screenblock->SetTile(x, y, screenmap[y * 30 + x] - 1);
        }
    }

    textRenderer = new TextRenderer(1, 2, 30, 0);
    nameBlockPtr = textRenderer->CreateTextArea(13, 9, 8, 3);
    descBlockPtr = textRenderer->CreateTextArea(3, 15, 24, 5);

    Render();
    hardware::SetupDisplayControl(ENABLE_OBJ, ENABLE_BG0, ENABLE_BG1, ENABLE_BG2, ENABLE_BG3);
    hardware::SetupBackground(0, 0, 31, false, 1);

    graphics::FadeInBackground(2);
    graphics::FadeInObjects(1);

    previousJob = -1;
}

void CharacterMenuScene::OnExit()
{
    delete textRenderer;
}

void CharacterMenuScene::Update()
{

    if ( currentJob < (isSecretJobUnlocked ? 3 : 2) && hardware::IsButtonPressed(KEY_RIGHT))
        currentJob++;
    if ( currentJob > 0 && hardware::IsButtonPressed(KEY_LEFT))
        currentJob--;

    if ( hardware::IsButtonPressed(KEY_A))
    {
        GetSceneManager()->SetScene(new GameScene(jobs[currentJob].unitType), true);
        graphics::FadeOutObjects(0);
        graphics::FadeOutBackground(1);
    }
}

void CharacterMenuScene::Render()
{

    static const int CURSOR_Y = 68;
    static const int CURSOR_X = 42;

    static const int SKILL_ICON_Y = 11 * 8 + 2;

    static const int SKILL_1_ICON_X = 6 * 8;
    static const int SKILL_2_ICON_X = 13 * 8;
    static const int SKILL_3_ICON_X = 20 * 8;

    if ( previousJob != currentJob)
    {
        textRenderer->ClearTextArea(nameBlockPtr, 8 * 3);
        textRenderer->ClearTextArea(descBlockPtr, 24 * 5);

        textRenderer->RenderString<8>(normalFont, nameBlockPtr, jobs[currentJob].Name, strlen(jobs[currentJob].Name), 6, 255, 254);
        textRenderer->RenderString<24>(normalFont, descBlockPtr, jobs[currentJob].DescriptionLine1, strlen(jobs[currentJob].DescriptionLine1), 6, 254, 253);
        textRenderer->RenderString<24>(normalFont, descBlockPtr, jobs[currentJob].DescriptionLine2, strlen(jobs[currentJob].DescriptionLine2), 8 + 8, 254, 253);
        textRenderer->RenderString<24>(normalFont, descBlockPtr, jobs[currentJob].DescriptionLine3, strlen(jobs[currentJob].DescriptionLine3), 10 + 16, 254, 253);

        hardware::SetSprite8(0, CURSOR_X + 48 * currentJob, CURSOR_Y, POINTER_ID, 0, 1);

        hardware::SetSprite8(1, SKILL_1_ICON_X, SKILL_ICON_Y, 32, 0, 2);
        hardware::SetSprite8(2, SKILL_2_ICON_X, SKILL_ICON_Y, 36, 0, 2);
        hardware::SetSprite8(3, SKILL_3_ICON_X, SKILL_ICON_Y, 40, 0, 2);

        previousJob = currentJob;
    }

    UpdateObjects();
}
