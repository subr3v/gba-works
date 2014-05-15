/********************************************************************
	created:	2014/03/12
	created:	12:3:2014   17:35
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\Unit.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	Unit
	file ext:	h
	author:		Stefano Musumeci

	purpose:	Unit class.
*********************************************************************/
#ifndef Unit_h__
#define Unit_h__

#include "GameObject.h"
#include "UnitDescription.h"

class Command;
class Game;

class Unit : public GameObject
{
    public:
        Unit(UnitType unitType, GameController* gameController);
        virtual ~Unit();

        Command* ThinkCommand();

        int GetMaxHealth() { return maxHealthPoints; }
        int GetHealth() { return healthPoints; }
        int GetMaxMagic() { return maxMagicPoints; }
        int GetMagic() { return magicPoints; }

        int GetExperience();
        int GetNeededExperience();

        int GetExperienceGiven();

        void UseItem(ItemType item);

        void GiveExperience(int16_t experience);
        void LevelUpTo(int16_t targetLevel);

        void ModifyHealth(int32_t health);
        void ModifyMagic(int32_t magic);
        int GetFood() { return food; }
        int GetMaxFood() { return MAX_FOOD; }

        int GetGold() { return gold; }
        void ModifyGold(int32_t amount) { this->gold += amount; }

        void UpgradeRandom();

        void GiveFood(int32_t food);
        void DamageUnit(Unit* damagedUnit);

        void InflictPoison();
        void InflictImmobilize();
        void InflictSleep();

        bool IsImmobilized();
        bool IsPoisoned();

        bool HasTrapNearby();

        void ChangeAttackEffect(AttackEffect attackEffect);
        AttackEffect GetAttackEffect();

        void EnableHumanControlled(bool enabled) { isControlledByHuman = enabled; }
        bool IsHumanControlled() { return isControlledByHuman; }

        int16_t GetAttack() { return physicalAttack; }
        int16_t GetMagicAttack() { return magicalAttack; }
        int16_t GetDefense() { return physicalDefense; }
        int16_t GetMagicDefense() { return magicalDefense; }
        int GetNumberOfUpgrades() { return numberOfUpgrades; }

        int GetUpgradeCost();

        int GetLevel();

        UnitType GetUnitType();
    private:
        bool IsCellEmpty(uint16_t x, uint16_t y);
        Command* ThinkHuman();
        Command* ThinkAI();

        void LevelUp();
        // Stat points.
        float maxHealthPoints;
        float maxMagicPoints;
        int16_t healthPoints;
        int16_t magicPoints;
        float physicalAttack;
        float magicalAttack;
        float physicalDefense;
        float magicalDefense;

        int gold;

        // Progress points.
        int experience;
        int level;

        short poisonTimer;
        short immobilizeTimer;
        short sleepTimer;

        ItemType itemToUse;
        AttackEffect attackEffect;

        short numberOfUpgrades;

        Unit(const Unit& unit); // Copy constructor disallowed.
        Unit& operator=(const Unit& unit); // Copy operator disallowed.
        bool isControlledByHuman;
        int stepTimer;
        int food;
        UnitType unitType;
        static const int MAX_FOOD = 100;

        UnitDescription unitDescription;
};


#endif // Unit_h__
