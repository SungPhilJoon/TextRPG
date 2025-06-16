#pragma once

#include <functional>
#include <iostream>
#include <vector>
#include <unordered_map>

class DataManager;
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

class DataInitializer
{
private:
    Parser* parser;
    
    std::vector<std::function<void(DataManager&, Parser&)>> initializer;

public:
    DataInitializer();
    void Execute(DataManager& dataManager);
};

class DataManager
{
private:
    DataInitializer* initializer;
    
    static DataManager* instance;

public:
    DataManager();

    ~DataManager();
    
    DataContainer<PlayerData> playerData;
    DataContainer<MonsterData> monsterData;
    DataContainer<ItemData> itemData;

    static DataManager* Instance();

    void Initialize();

    void PrintData();
};
