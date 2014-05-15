/********************************************************************
	created:	2014/04/07
	created:	7:4:2014   23:54
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\CharacterMenuScene.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	CharacterMenuScene
	file ext:	h
	author:		Stefano Musumeci

	purpose:
*********************************************************************/
#ifndef CharacterMenuScene_h__
#define CharacterMenuScene_h__

#include "Scene.h"
#include "stdint.h"
#include "GameConstants.h"

class TextRenderer;

class CharacterMenuScene : public Scene
{

    public:
        CharacterMenuScene();

        virtual void Update();
        virtual void Render();
        virtual void OnEnter();
        virtual void OnExit();

    private:

        struct JobInformation
        {
            const char* Name;
            const char* DescriptionLine1;
            const char* DescriptionLine2;
            const char* DescriptionLine3;
            int SkillIcons[3];
            UnitType unitType;

            JobInformation() { }

            JobInformation(UnitType unitType, const char* name, const char* description1, const char* description2, const char* description3, int skill1, int skill2, int skill3)
            {
                this->unitType = unitType;
                this->Name = name;
                this->DescriptionLine1 = description1;
                this->DescriptionLine2 = description2;
                this->DescriptionLine3 = description3;
                this->SkillIcons[0] = skill1;
                this->SkillIcons[1] = skill2;
                this->SkillIcons[2] = skill3;
            }
        };

        JobInformation jobs[4];

        bool isSecretJobUnlocked;
        int currentJob;
        int previousJob;

        TextRenderer* textRenderer;
        uint16_t* nameBlockPtr;
        uint16_t* descBlockPtr;
        static const int POINTER_ID = 0;
};


#endif // CharacterMenuScene_h__
