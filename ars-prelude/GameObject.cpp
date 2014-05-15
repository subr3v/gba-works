#include "GameObject.h"
#include <cstddef>

GameObject::~GameObject()
{

}

GameObject::GameObject( GameController* gameController, GameObjectType type )
{
    this->gameController = gameController;
    this->type = type;
    this->isDead = false;
    this->currentDirection = South;
    this->currentFrame = 0;
    this->animationTimer = 0;
    this->currentFrameAnimation = 0;
    this->currentAnimation = Still;
}

void GameObject::SetSpriteIdentifier( SpriteIdentifier identifier )
{
    spriteIdentifier = identifier;
}

SpriteIdentifier GameObject::GetSpriteIdentifier()
{
    return spriteIdentifier;
}

int GameObject::GetCurrentFrame()
{
    return currentFrame;
}

Point16 GameObject::GetGridPosition()
{
    return Point16(position.x / TILE_SIZE, position.y / TILE_SIZE);
}

GameObjectType GameObject::GetType()
{
    return type;
}

void GameObject::Kill()
{
    isDead = true;
}

bool GameObject::IsDead()
{
    return isDead;
}

void GameObject::Update()
{
    animationTimer++;
    if ( animationTimer > spriteAnimationFrames[currentAnimation].frameTime )
    {
        animationTimer = 0;
        currentFrameAnimation++;
        if ( currentFrameAnimation >= spriteAnimationFrames[currentAnimation].frameCount )
        {
            if ( spriteAnimationFrames[currentAnimation].isLooped )
                currentFrameAnimation = 0;
            else
                currentFrameAnimation = spriteAnimationFrames[currentAnimation].frameCount - 1;
        }
    }
    currentFrame = spriteAnimationFrames[currentAnimation].frames[currentFrameAnimation];
}

bool GameObject::SupportsDirection()
{
    return spriteAnimationFrames[currentAnimation].supportsDirection;
}

void GameObject::DisplayAnimation( SpriteAnimation animation )
{
    currentFrameAnimation = 0;
    animationTimer = 0;
    currentAnimation = animation;
}

GameController* GameObject::GetGameController()
{
    return gameController;
}

SpriteAnimation GameObject::GetCurrentAnimation()
{
    return currentAnimation;
}

Item* GameObject::AsItem()
{
    if ( type == Object_Item)
        return (Item*)this;
    return NULL;
}

Unit* GameObject::AsUnit()
{
    if ( type == Object_Unit)
        return (Unit*)this;
    return NULL;
}

void GameObject::Move( Direction direction, int magnitude )
{
    if ( direction  & East  )
        position.x += magnitude;
    if ( direction  & West  )
        position.x -= magnitude;
    if ( direction  & North )
        position.y -= magnitude;
    if ( direction & South )
        position.y += magnitude;
}

