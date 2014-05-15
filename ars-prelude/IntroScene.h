/********************************************************************
	created:	2014/04/02
	created:	2:4:2014   14:59
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\IntroScene.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	IntroScene
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef IntroScene_h__
#define IntroScene_h__

#include "stdint.h"
#include "Scene.h"

class IntroScene : public Scene
{
        virtual void Update();
        virtual void Render();
        virtual void OnEnter();

    private:
        void FadeInOutImage(const uint16_t* palette, const uint16_t* tiles, int waitTime);

        int32_t randomSeed;
        uint16_t visibleCounter;
        uint16_t buttonX;
        uint16_t buttonY;
};

#endif // IntroScene_h__
