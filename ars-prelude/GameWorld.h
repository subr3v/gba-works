/********************************************************************
	created:	2014/04/02
	created:	2:4:2014   12:27
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\GameWorld.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	GameWorld
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef GameWorld_h__
#define GameWorld_h__

#include "GameConstants.h"
#include "GameObject.h"
#include "Unit.h"

class DungeonMap;

class GameWorld
{
    public:
        GameObject* objects[MAX_OBJECTS];
        DungeonMap* map;
        Unit* player;
        bool isMapBeaten;

        GameWorld(DungeonMap* map)
        {
            for (int i = 0; i < MAX_OBJECTS; i++)
                objects[i] = NULL;
            this->map = map;
            player = NULL;
            isMapBeaten = false;
        }

        void SetPlayer(Unit* player)
        {
            this->player = player;
        }

        void StartLevel()
        {
            this->isMapBeaten = false;
            for (int i = 0; i < MAX_OBJECTS; i++)
            {
                if (objects[i] != NULL && objects[i] != player) // Make sure we don't delete the player.
                    delete objects[i];
                objects[i] = NULL;
            }
            AddObject(player); // We add the player again.
        }

        void Update()
        {
            for (size_t i = 0; i < MAX_OBJECTS; i++)
            {
                if (objects[i] != NULL)
                {
                    objects[i]->Update();
                    if ( objects[i]->IsDead() )
                        RemoveObject(i);
                }
            }
        }

        void AddObject(GameObject* object)
        {
            for (int i = 0; i < MAX_OBJECTS; i++)
            {
                if (objects[i] == NULL)
                {
                    objects[i] = object;
                    break;
                }
            }
        }

        void RemoveObject(int index)
        {
            delete objects[index];
            objects[index] = NULL;
        }
};

#endif // GameWorld_h__
