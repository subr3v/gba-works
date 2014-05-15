/********************************************************************
	created:	2014/03/27
	created:	27:3:2014   23:52
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\GameObject.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	GameObject
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef GameObject_h__
#define GameObject_h__

#include "stdint.h"
#include "Point.h"
#include "GameConstants.h"
#include "ObjectGraphics.h"

class GameController;

class Unit;
class Item;

class GameObject
{
    public:
        GameObject(GameController* gameController, GameObjectType type);
        virtual ~GameObject();

        void Update();
        bool IsDead();
        void Kill();

        GameObjectType GetType();
        GameController* GetGameController();

        void DisplayAnimation(SpriteAnimation animation);
        void SetSpriteIdentifier(SpriteIdentifier identifier);

        Point16 GetGridPosition();
        int GetCurrentFrame();
        SpriteIdentifier GetSpriteIdentifier();
        bool SupportsDirection();
        SpriteAnimation GetCurrentAnimation();

        Item* AsItem();
        Unit* AsUnit();

        void Move(Direction direction, int magnitude);

        Point16 position;
        Direction currentDirection;
    protected:
        int currentFrame;
        GameController* gameController;
    private:
        bool isDead;
        GameObjectType type;
        SpriteIdentifier spriteIdentifier;
        SpriteAnimation currentAnimation;
        int animationTimer;
        int currentFrameAnimation;
};

#endif // GameObject_h__
