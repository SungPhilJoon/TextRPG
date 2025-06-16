#include "DataManager.h"
#include "GameData.h"
#include "Parser.h"

template<typename TData = GameData>
DataContainer<TData>::~DataContainer()
{
    for (auto data : dataContainer)
    {
        delete data;
    }
}

void DataContainer<PlayerData>::addData(PlayerData* data)
{
    int containerIndex = dataContainer.size();
    dataIndexById[data->getIndex()] = containerIndex;
    this->dataContainer.push_back(data);
}

PlayerData* DataContainer<PlayerData>::getData(int id)
{
    if (dataIndexById.find(id) == dataIndexById.end())
    {
        return nullptr;
    }

    return dataContainer[dataIndexById[id]];
}

void DataContainer<MonsterData>::addData(MonsterData* data)
{
    int containerIndex = dataContainer.size();
    dataIndexById[data->getIndex()] = containerIndex;
    this->dataContainer.push_back(data);
}

MonsterData* DataContainer<MonsterData>::getData(int id)
{
    if (dataIndexById.find(id) == dataIndexById.end())
    {
        return nullptr;
    }

    return dataContainer[dataIndexById[id]];
}

void DataContainer<ItemData>::addData(ItemData* data)
{
    int containerIndex = dataContainer.size();
    dataIndexById[data->getIndex()] = containerIndex;
    this->dataContainer.push_back(data);
}

ItemData* DataContainer<ItemData>::getData(int id)
{
    if (dataIndexById.find(id) == dataIndexById.end())
    {
        return nullptr;
    }

    return dataContainer[dataIndexById[id]];
}

DataInitializer::DataInitializer()
{
    parser = new CSVParser();

    initializer.emplace_back(PlayerData::InitializeData);
    initializer.emplace_back(MonsterData::InitializeData);
    initializer.emplace_back(ItemData::InitializeData);
}

void DataInitializer::Execute(DataManager& dataManager)
{
    for (const auto& init : initializer)
    {
        init(dataManager, *parser);
    }
}

DataManager::DataManager() : initializer(new DataInitializer()) { }

DataManager* DataManager::instance = nullptr;

DataManager* DataManager::Instance()
{
    if (instance == nullptr)
    {
        instance = new DataManager();
    }

    return instance;
}

DataManager::~DataManager()
{
    delete initializer;
}

void DataManager::Initialize()
{
    initializer->Execute(*this);
}

#define Test

void DataManager::PrintData()
{
#ifdef Test
    playerData.printData();
    monsterData.printData();
    itemData.printData();
#endif
}
