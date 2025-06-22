#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "Item.h"
#include"Component.h"

class PlayerData;
class MonsterData;



class Actor
{
protected:
    std::string name;

    int level = 1;

    int baseHP = 0;
    int baseDamage = 0;
    int baseDefense = 0;

    int currentHP = 0;

    int incDamage = 0;
    int incDefense = 0;

    bool isDead()
    {
        return currentHP <= 0;
    }

public:
    void SetName(const std::string& name)
    {
        this->name = name;
    }

    int GetHP() const
    {
        return currentHP;
    }

    int GetBaseHP() const // 기본 체력(baseHP) 반환, 회복/아이템 사용 조건 등에서 기준값으로 사용됨
    {
        return baseHP;
    }

    int GetDamage() const
    {
        return baseDamage * level + incDamage;
    }

    int GetLevel() const
    {
        return level;
    }

    int GetDefense() const
    {
        return baseDefense * level + incDefense;
    }

    void IncreaseHP(int hp)
    {
        this->currentHP = std::min(this->currentHP + hp, baseHP);
    }

    void DecreaseHP(int hp)
    {
        this->currentHP = std::max(0, this->currentHP - hp);
    }

    void IncreaseDamage(int damage)
    {
        this->incDamage += damage;
    }

    void DecreaseDamage(int damage)
    {
        this->incDamage = std::max(0, this->incDamage - damage);
    }

    void IncreaseDefense(int defense)
    {
        this->incDamage += defense;
    }

    void DecreaseDefense(int defense)
    {
        this->incDefense = std::max(0, this->incDefense - defense);
    }

    void increaseLevel()
    {
        ++this->level;
    }

    virtual void attack(Actor& target) = 0;
    virtual void damaged(const Actor& attacker) = 0;
};

class Player : public Actor, public ItemUseable
{
private:
    PlayerData* data;

    InventoryComponent* inventory; // 인벤토리 
    CurrencyComponent* wallet;  //재화관리 

    int exp = 0;
    int killCount = 0;

public:
    Player();

    void setData(PlayerData* data);
    void levelUp();
    bool IsNicknameEmpty();

    // 250620 경험치
    void GainExp(int amount);
    void TryLevelUp();
    int GetExp() const { return exp; }

    // ---- items ----//
    const std::vector<Item*>& getInventory() const;
    bool reduceItem(int idx ,int amount=1);
    void addItem(const Item& item); 
    void useItemDuringCombat(); // 250622 전투 중 자동 아이템 사용 함수

    // --- currency ---///
    int getGold()const; 
    void addGold(int amount =1);
    bool reduceGold(int amount =1);

    virtual void attack(Actor& target) override;
    virtual void damaged(const Actor& attacker) override;

    virtual void useItem(Item&) override;

    void addKillCount()
    {
        killCount++;
    }

    int getMonsterKillCount() const
    {
        return killCount;
    }
};

class Monster : public Actor
{
private:
    MonsterData* data;

public:
    virtual void attack(Actor& target) override;
    virtual void damaged(const Actor& attacker) override;

    // 250620 플레이어 레벨 기반 능력치 설정용으로 수정, playerLevel을 넘기기위해 추가
    void setData(MonsterData* data, int playerLevel);

    const std::string getName() const;
};
