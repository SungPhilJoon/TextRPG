#include "GameData.h"
#include "DataManager.h"
#include "Parser.h"

void PlayerData::InitializeData(DataManager& dataManager, Parser& parser)
{
    parser.initialize("PlayerData.csv");
        
    int lineCount = parser.getLine();
    for (int i = 0; i < lineCount; ++i)
    {
        PlayerData* data = new PlayerData();

        int level = parser.getIntData(i, "level");
        data->index = level;
        data->level = level;
        data->hp = parser.getIntData(i, "hp");
        data->damage = parser.getIntData(i, "damage");
        data->defense = parser.getIntData(i, "defense");

        dataManager.playerData.addData(data);
    }
}

void MonsterData::InitializeData(DataManager& dataManager, Parser& parser)
{
    parser.initialize("MonsterData.csv");
        
    int lineCount = parser.getLine();
    for (int i = 0; i < lineCount; ++i)
    {
        MonsterData* data = new MonsterData();

        data->index = parser.getIntData(i, "index");
        data->level = parser.getIntData(i, "level");
        data->hp = parser.getIntData(i, "hp");
        data->damage = parser.getIntData(i, "damage");
        data->defense = parser.getIntData(i, "defense");
        data->name = parser.getStringData(i, "name");

        dataManager.monsterData.addData(data);
    }
}

void ItemData::InitializeData(DataManager& dataManager, Parser& parser)
{
    parser.initialize("ItemData.csv");
        
    int lineCount = parser.getLine();
    for (int i = 0; i < lineCount; ++i)
    {
        ItemData* data = new ItemData();

        data->index = parser.getIntData(i, "index");
        data->name = parser.getStringData(i, "name");
        data->value = parser.getIntData(i, "value");

        dataManager.itemData.addData(data);
    }
}