#pragma once

#include <string>
#include <vector>


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

    int getIndex() const
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

    static void InitializeData(DataManager& dataManager, Parser& parser);
};

class MonsterData : public GameData
{
private:
    int hp;
    int damage;
    int defense;
    int level;
    std::string name;

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
    const std::string getName() const
    {
        return name;
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
