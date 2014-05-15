#include "SceneManager.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "CharacterMenuScene.h"
#include "ShopScene.h"
#include "CutsceneScene.h"
#include "GameConstants.h"

#include <cstdlib>
#include "gba.h"

int main()
{
    ClearObjects();
    RandomSeed(0);
    SceneManager sceneManager;
    Scene* scene = NULL;
    scene = new IntroScene();
    sceneManager.SetScene(scene, false);
    sceneManager.Run();
    return 0;
}

