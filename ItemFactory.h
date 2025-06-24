#pragma once
#pragma once
#include <unordered_map>
#include <memory>

class ItemData;
class Item;

enum class ItemType;

// Creator 
class ItemCreator {
public:
    virtual ~ItemCreator() = default;
    virtual Item* createItem(ItemData* data, int count = 1) const = 0;
};

// Concrete Creators
class PotionCreator : public ItemCreator {
public:
    Item* createItem(ItemData* data, int count = 1) const override;
};

class UpgradeDamageCreator : public ItemCreator {
public:
    Item* createItem(ItemData* data, int count = 1) const override;
};

// Factory  
class ItemFactory {
private:
    std::unordered_map<ItemType, std::unique_ptr<ItemCreator>> creators;
    ItemFactory();
    ItemType FindTypebyIndex(int index);
public:
    static ItemFactory& getInstance() {
        static ItemFactory instance;  // C++11 이후 스레드 안전
        return instance;
    }
    Item* createItem(int idx, int count = 1);
};
