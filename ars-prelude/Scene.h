/********************************************************************
	created:	2014/04/01
	created:	1:4:2014   19:47
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\Scene.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	Scene
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef Scene_h__
#define Scene_h__

class SceneManager;

class Scene
{
    public:
        Scene() { }
        virtual ~Scene() { }
        virtual void OnEnter() { }
        virtual void OnExit() { }
        virtual void Update() = 0;
        virtual void Render() = 0;
    protected:
        SceneManager* GetSceneManager() { return sceneManager; }
        friend class SceneManager;
    private:
        SceneManager* sceneManager;
};


#endif // Scene_h__
