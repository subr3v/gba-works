/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   19:45
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\SceneManager.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	SceneManager
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef SceneManager_h__
#define SceneManager_h__

class Scene;

class SceneManager
{
    public:
        SceneManager();
        ~SceneManager();
        void SetScene(Scene* scene, bool destroyPreviousScene);
        void Run();
    private:
        Scene* currentScene;
        Scene* nextScene;
        bool destroyPreviousScene;
};

#endif // SceneManager_h__
