#include "ItemFactory.h"
#include "Item.h"         
#include "GameData.h"
#include<unordered_map>

#include "DataManager.h"


Item* PotionCreator::createItem(ItemData* data, int count) const {
    return new PotionItem(data, count);
}


Item* UpgradeDamageCreator::createItem(ItemData* data, int count) const {
    return new UpgradeDamageItem(data, count);
}


ItemFactory::ItemFactory() {
    creators[ItemType::Potion] = std::make_unique<PotionCreator>();
    creators[ItemType::UpgradeDamage] = std::make_unique<UpgradeDamageCreator>();
}

ItemType ItemFactory::FindTypebyIndex(int index)
{
    int type = index / 10000;
    switch (type) {
    case 1: return ItemType::UpgradeDamage;
    case 2: return ItemType::Potion;
    default: return ItemType::None;
    }
    return ItemType::None;
}

Item* ItemFactory::createItem(int idx, int count)
{
    int itemidx =  idx / 10000;
    ItemData* data = Manager<DataManager>::Instance()->itemData.getData(itemidx-1);
    if (!data)return nullptr;
    ItemType type = FindTypebyIndex(data->getIndex());
    auto it = creators.find(type);
    if (it != creators.end()) {
        Item* returnItem = it->second->createItem(data, count);
        return returnItem;
    }
    return nullptr;
}

