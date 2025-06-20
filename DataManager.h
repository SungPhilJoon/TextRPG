#pragma once

#include <functional>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Manager.h"

class DataInitializer;
class GameData;
class PlayerData;
class MonsterData;
class ItemData;
class Parser;

template<typename TData = GameData>
class DataContainer
{
private:
    std::vector<TData*> dataContainer;
    std::unordered_map<int, int> dataIndexById;

public:
    ~DataContainer();
    
    void addData(TData* data);
    std::vector<TData*> getDataContainer();

    TData* getData(int id);

#define Test
    
    void printData()
    {
#ifdef Test
        for (auto data : this->dataContainer)
        {
            std::cout << data->getIndex() << std::endl;
        }
        std::cout << std::endl;
#endif
    }
};

class DataManager : public Manager<DataManager>
{
private:
    DataInitializer* initializer;

public:
    DataManager();

    virtual ~DataManager() override;
    
    DataContainer<PlayerData> playerData;
    DataContainer<MonsterData> monsterData;
    DataContainer<ItemData> itemData;

    virtual void InitGame() override;
    virtual void EnterGame() override;
    virtual bool UpdateGame() override;
    virtual void ExitGame() override;

    void PrintData();
};

class DataInitializer
{
private:
    Parser* parser;
    
    std::vector<std::function<void(DataManager&, Parser&)>> initializer;

public:
    DataInitializer();
    void Execute(DataManager& dataManager);
};
