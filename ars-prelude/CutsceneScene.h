/********************************************************************
	created:	2014/04/17
	created:	17:4:2014   12:49
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\CutsceneScene.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	CutsceneScene
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef CutsceneScene_h__
#define CutsceneScene_h__

#include "Scene.h"
#include "GameRenderer.h"

class TextRenderer;
class GameWorld;

enum CutsceneMap
{
    Cutscene_Intro,
    Cutscene_Campfire,
    Cutscene_Crystal,
};

struct DialoguePart
{
    UnitType unit;
    const char* dialogLines[3];

    DialoguePart(UnitType unit, const char* line1, const char* line2, const char* line3)
    {
        this->unit = unit;
        this->dialogLines[0] = line1;
        this->dialogLines[1] = line2;
        this->dialogLines[2] = line3;
    }
};

struct CutsceneDialog
{
    const DialoguePart* parts;
    int partsCount;
    CutsceneMap cutsceneMap;

    CutsceneDialog(CutsceneMap cutsceneMap = Cutscene_Intro, const DialoguePart* parts = NULL, int partsCount = 0)
    {
        this->cutsceneMap = cutsceneMap;
        this->parts = parts;
        this->partsCount = partsCount;
    }
};

class CutsceneScene : public Scene
{
    public:
        CutsceneScene(int cutsceneNumber, Scene* nextScene);
        virtual void OnEnter();
        virtual void OnExit();
        virtual void Update();
        virtual void Render();
    private:


        enum CutsceneState
        {
            State_None,
            State_UnitsWalkingIn,
            State_UnitsWalkingOut,
            State_UnitsTalking,
            State_End,
        };

        void ChangeState(CutsceneState newState);
        CutsceneState currentState;

        void SetUnitsPosition(int yPosition);
        void ShowText(UnitType talkingUnit, const char**);

        void RenderText();

        Scene* nextScene;
        GameWorld* world;
        GameRenderer gameRenderer;
        TextRenderer* textRenderer;
        CutsceneMap currentMap;

        uint16_t* textArea;
        const char* dialogLines[3];
        unsigned int currentCharacter;
        int currentLine;
        CutsceneDialog currentDialog;
        int currentPart;

        int timer;
        int moveTimer;
        int moveStartY;
        int moveEndY;
        CutsceneState movementNextState;
        Direction talkingDirection;

        const static int MOVEMENT_TIME = 120;

        Unit* mage;
        Unit* knight;
        Unit* thief;
};


#endif // CutsceneScene_h__
