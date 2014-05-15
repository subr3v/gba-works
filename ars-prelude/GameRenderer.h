/********************************************************************
	created:	2014/03/22
	created:	22:3:2014   15:17
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\GameRenderer.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	GameRenderer
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef GameRenderer_h__
#define GameRenderer_h__

#include "Point.h"
#include "System.h"
#include "GameConstants.h"
#include "stdint.h"
#include "GameFonts.h"
#include <cstring>

class Unit;
class DungeonMap;
class GameObject;

class GameRenderer
{
    public:
        GameRenderer();
        ~GameRenderer();

        void Init();
        void Shutdown();
        void LoadMap(DungeonMap& dungeonMap, DungeonSetting setting);

        void ClearText();
        void DisplayText(char* text, int textLength);
        void DisplayText(char* text);

        void DisplayDamage(Point16 position, int amount);
        void DisplayLevelUp(Point16 position);

        void ShowDungeonLevel(int level);

        void SetCamera(int x, int y);
        void RenderDungeon();
        void RenderInterface(bool showDialog, int playerHp, int playerMaxHp, int playerMp, int playerMaxMp, int playerFood, int playerMaxFood, int playerExperience, int playerNeededExperience);
        void RenderObjects(GameObject* objects[MAX_OBJECTS]);
    private:
        void RenderDialogue();

        Point16 cameraPosition;
        char textboxText[3][32];

        struct ObjectSprite
        {
            public:
                uint16_t spriteIdentifier;
                uint16_t frameNumber;
                uint16_t rowNumber;
                uint16_t targetCharblock;

                bool operator==(const ObjectSprite& other)
                {
                    return spriteIdentifier == other.spriteIdentifier && frameNumber == other.frameNumber && rowNumber == other.rowNumber && targetCharblock == other.targetCharblock;
                }
        };

        int dialogRow;
        TextRenderer* textRenderer;
        ObjectSprite sprites[MAX_OBJECTS];
        ObjectSprite previousSprites[MAX_OBJECTS];
        bool redrawMap;
        short* virtualScreenblockBuffer;
        int virtualScreenblockWidth;
        int virtualScreenblockHeight;

        hardware::Screenblock* mapScreenblock;
        hardware::Screenblock* guiScreenblock;
        hardware::Screenblock* textScrenblock;

        uint16_t previousExperience;
        uint16_t previousHp, previousMp, previousFood;
        uint16_t previousMaxHp, previousMaxMp, previousMaxFood;
        bool isDialogVisible;
        uint16_t* dialogCharblock;
        uint16_t* hpCharblock;
        uint16_t* mpCharblock;
        uint16_t* expCharblock;
        uint16_t* dungeonLvlCharblock;

        int gameTextTimer;
        short gameTextSprites[5];
        short gameTextSpriteOffsets[5];
        Point16 gameTextPosition;
        static const int GAME_TEXT_DISPLAY_TIME = 60;

        int dungeonLevelTimer;
        static const int DUNGEON_LEVEL_TIME = 45;

        static const int GUI_BACKGROUND = 1;
        static const int GUI_CHARBLOCK = 2;
        static const int GUI_SCREENBLOCK = 13;

        static const int TEXT_BACKGROUND = 2;
        static const int TEXT_CHARBLOCK = 3;
        static const int TEXT_SCREENBLOCK = 14;

        static const int MAP_BACKGROUND = 0;
        static const int MAP_CHARBLOCK = 0;
        static const int MAP_SCREENBLOCK = 15;

        static const int UNITS_CHARBLOCK = 4;

        static const int EMPTY_GUI_TILE = 10;
        static const int EMPTY_TEXT_TILE = 255;
        static const int EMPTY_MAP_TILE = 255;
};

#endif // GameRenderer_h__
