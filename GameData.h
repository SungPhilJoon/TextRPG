#pragma once

#include <string>

class DataManager;
class Parser;

class GameData
{
protected:
    int index;

public:
    void setIndex(int index)
    {
        this->index = index;
    }

    virtual int getIndex() const
    {
        return index;
    }
};

class PlayerData : public GameData
{
private:
    int hp;
    int damage;
    int defense;
    int level;
    int needExp;

    static int MAX_LEVEL;

public:
    int getHP() const
    {
        return hp;
    }
    int getDamage() const
    {
        return damage;
    }
    int getDefense() const
    {
        return defense;
    }
    int getLevel() const
    {
        return level;
    }

    static bool IsMaxLevel(int level)
    {
        return MAX_LEVEL <= level;
    }

    static void InitializeData(DataManager& dataManager, Parser& parser);
};

class MonsterData : public GameData
{
public:
    enum class Type
    {
        Normal,
        Boss
    };
    
private:
    // int hp;
    // int damage;
    // int defense;
    int level;
    int appearLevel;
    int exp;
    std::string name;
    Type type;

public:
    // int getHP() const
    // {
    //     return hp;
    // }
    // int getDamage() const
    // {
    //     return damage;
    // }
    // int getDefense() const
    // {
    //     return defense;
    // }
    
    int getLevel() const
    {
        return level;
    }
    int getExp() const
    {
        return exp;
    }
    const std::string getName() const
    {
        return name;
    }

    Type getType() const
    {
        return type;
    }

    virtual int getIndex() const override
    {
        return appearLevel;   
    }

    static void InitializeData(DataManager& dataManager, Parser& parser);
};

class ItemData : public GameData
{
private:
    std::string name;
    int value;

public:
    std::string getName() const
    {
        return name;
    }
    int getValue() const
    {
        return value;
    }

    static void InitializeData(DataManager& dataManager, Parser& parser);
};
