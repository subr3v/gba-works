#include "SceneManager.h"
#include "Scene.h"
#include "System.h"
#include <cstddef>

SceneManager::SceneManager()
{
    currentScene = NULL;
    nextScene = NULL;
    destroyPreviousScene = false;
}

SceneManager::~SceneManager()
{

}

void SceneManager::Run()
{
    while (true)
    {
        if ( nextScene != NULL )
        {
            if ( currentScene != NULL && destroyPreviousScene )
            {
                currentScene->OnExit();
                delete currentScene;
            }
            else if ( currentScene != NULL && destroyPreviousScene == false )
            {
                currentScene->OnExit();
            }
            currentScene = nextScene;
            nextScene = NULL;
            currentScene->OnEnter();
        }

        if ( currentScene != NULL )
        {
            hardware::WaitVSync();
            hardware::UpdateInput();

            currentScene->Update();
            currentScene->Render();
        }
    }
}

void SceneManager::SetScene( Scene* scene, bool destroyPreviousScene )
{
    scene->sceneManager = this;
    nextScene = scene;
    this->destroyPreviousScene = destroyPreviousScene;
}
