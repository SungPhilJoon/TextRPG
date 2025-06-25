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

    int GetBaseHP() const // �⺻ ü��(baseHP) ��ȯ, ȸ��/������ ��� ���� ��� ���ذ����� ����
    {
        return baseHP;
    }

    int GetBaseDamage() const
    {
        return baseDamage;
    }

    int GetDamage() const
    {
        return baseDamage + incDamage;
    }

    int GetIncDamage() const
    {
        return incDamage;
    }

    std::string GetIncDamageString() const
    {
        if (GetIncDamage() > 0)
            return " (+" + std::to_string(GetIncDamage()) + ")";
        return "";
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

    InventoryComponent* inventory; // �κ��丮 
    CurrencyComponent* wallet;  //��ȭ���� 

    int exp = 0;
    int killCount = 0;

    void levelUp();
    void TryLevelUp();
    int GetExp() const { return exp; }

public:
    Player();

    void setData(PlayerData* data);
    bool IsNicknameEmpty();

    // 250620 ����ġ
    void GainExp(int amount);

    // ---- items ----//
    const std::vector<Item*>& getInventory() const;
    bool reduceItem(int idx ,int amount=1);
    void addItem(int itemidx, int amount=1); 
    void useItemDuringCombat(); // 250622 ���� �� �ڵ� ������ ��� �Լ�

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
    bool bIsBoss = false;

public:
    virtual void attack(Actor& target) override;
    virtual void damaged(const Actor& attacker) override;

    // 250620 �÷��̾� ���� ��� �ɷ�ġ ���������� ����, playerLevel�� �ѱ������ �߰�
    void setData(MonsterData* data, int playerLevel);

    const std::string getName() const;

    void SetIsBoss(bool b) { bIsBoss = b;  }
    bool IsBoss() const { return bIsBoss;  }
};
