#include "IntroScene.h"
#include "System.h"

#include "alcapa_logo.h"
#include "licensed.h"
#include "ars_intro.h"
#include "press_to_play.h"

#include "GraphicsUtil.h"
#include "gba.h"

#include "GameScene.h"
#include "SceneManager.h"
#include "CharacterMenuScene.h"

void IntroScene::Update()
{
    if (buttonY < 72)
        buttonY++;
    else
    {
        visibleCounter++;
        if (visibleCounter > 60)
            visibleCounter = 0;

        if (hardware::IsButtonPressed(KEY_START))
        {
            graphics::FadeOutObjects(1);
            graphics::FadeOutBackground(2);
            GetSceneManager()->SetScene(new CharacterMenuScene(), true);
            RandomSeed(randomSeed);
        }
    }

    randomSeed++;
}

void IntroScene::Render()
{

    bool isStartVisible = visibleCounter <= 30;

    for (int x = 0; x < 13; x++)
    {
        if ( isStartVisible == false )
        {
            hardware::HideSprite(x);
            hardware::HideSprite(x + 13);
        }
        else
        {
            hardware::SetSprite8(x, buttonX + x * 8, buttonY, x, 0, 0, false, false, 0);
            hardware::SetSprite8(x + 13, buttonX + x * 8, buttonY + 8, x + 13, 0, 0, false, false, 0);
        }
    }

    UpdateObjects();
}

void IntroScene::OnEnter()
{
    const bool ENABLE_OBJ = true;
    const bool ENABLE_BG0 = true;
    const bool ENABLE_BG1 = false;
    const bool ENABLE_BG2 = false;
    const bool ENABLE_BG3 = false;

    // Setup the correct hardware registers.
    hardware::SetupDisplayControl(ENABLE_OBJ, ENABLE_BG0, ENABLE_BG1, ENABLE_BG2, ENABLE_BG3);
    hardware::SetupBackground(0, 0, 31, false, 0);

    // We just need to fade in / out a sequence of images here.
    uint16_t tileCounter = 0;
    for (int x = 0; x < 30; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            hardware::Screenblocks[31].SetTile(x, y, tileCounter++);
        }
    }

    FadeInOutImage(alcapa_logoPal, alcapa_logoTiles, 30);
    FadeInOutImage(licensedPal, licensedTiles, 35);
    FadeInOutImage(ars_introPal, ars_introTiles, 0);

    hardware::CopyPalette(hardware::OBJ_Palette, press_to_playPal);
    graphics::SetFadePaletteObjects(press_to_playPal);
    // Setup obj for "press start to play button"
    // The button is 13 tiles wide and 2 tiles tall

    buttonX = 72;
    buttonY = 0;
    visibleCounter = 0;
    ClearObjects();

    for (int x = 0; x < 13; x++)
    {
        hardware::CopyTile8((uint16_t*)(press_to_playTiles + 32 * x), hardware::Charblocks[4].GetTile8(x));
        hardware::CopyTile8((uint16_t*)(press_to_playTiles + 32 * x + (32 * 13)), hardware::Charblocks[4].GetTile8(x + 13));
    }
}

void IntroScene::FadeInOutImage( const uint16_t* palette, const uint16_t* tiles, int waitTime )
{
    graphics::SetFadePaletteBackground(palette);

    uint16_t tileCounter = 0;
    for (int x = 0; x < 30; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            hardware::CopyTile8((uint16_t*)(tiles + 32 * x + (32 * 30) * y), hardware::Charblocks[0].GetTile8(tileCounter++));
        }
    }
    graphics::FadeInBackground(3);
    if ( waitTime > 0 )
    {
        for (int i = 0; i < waitTime; i++)
            hardware::WaitVSync();
        graphics::FadeOutBackground(2);
    }
}
