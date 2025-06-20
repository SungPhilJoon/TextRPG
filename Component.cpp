#include<iostream>
#include "Component.h"
#include"Item.h"

#include"Actor.h"


void Component::initialize() {
    // 필요시 override
}
void Component::destroy() 
{
    // 필요시 override
}


int CurrencyComponent::getGold() const 
{
    return gold;
}

void CurrencyComponent::initialize() 
{
    gold = 100;
}

void CurrencyComponent::addGold(int amount) 
{
    gold += amount;
}

bool CurrencyComponent::reduceGold(int amount)
{
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    std::cout << "Not Enought Gold" << std::endl;
    return false;
}



int InventoryComponent::FindItem(const Item& target) const 
{
    auto it = std::find_if(items.begin(), items.end(),
        [&target](const Item* itemPtr) {
            return itemPtr && itemPtr->getName() == target.getName();
        });

    return (it != items.end()) ? static_cast<int>(std::distance(items.begin(), it)) : -1;
}

InventoryComponent::~InventoryComponent()
{
    destroy();
}

const std::vector<Item*> InventoryComponent::getInventory()
{
    return std::vector<Item*>();
}

void InventoryComponent::initialize()
{
    // 필요 시 구현
}

void InventoryComponent::destroy() 
{
    for (auto* item : items) {
        delete item;
    }
    items.clear();
}

void InventoryComponent::addItem(const Item& item) {
    int index = FindItem(item);
    if (index >= 0) {
        items[index]->addItem(item.getCount());
    }
    else {
        items.push_back(item.clone()); 
    }
}

bool InventoryComponent::reduceItem(int idx, int amount)
{
    idx -= 1; // 사용자 입력이 1부터 시작한다고 가정

    
    if (idx < 0 || idx >= items.size())
    {
        std::cout << "The item is not in your inventory." << std::endl;
        return false;
    }

    if (items[idx]->getValue() >= amount)
    {
        items[idx]->decreaseItem(amount);

        if (items[idx]->getCount() <= 0)
        {
            delete items[idx];
            items.erase(items.begin() + idx);
        }
        return true;
    }
    else
    {
        std::cout << "Not enough quantity" << std::endl;
        return false;
    }
}



