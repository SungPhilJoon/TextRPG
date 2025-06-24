#include<iostream>
#include "Component.h"
#include"Item.h"

#include"Actor.h"
#include "GameData.h"
#include <iterator>  

#include "ItemFactory.h"


void Component::initialize() {
    // �ʿ�� override
}
void Component::destroy() 
{
    // �ʿ�� override
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

int InventoryComponent::FindItem(const int idx) const
{
    for (int i = 0; i < items.size(); ++i)
    {
        int num =items[i]->getData()->getIndex();
        if (items[i]->getData()->getIndex() == idx)
        {
            return i;
        }
    }
    return -1;  // 못 찾으면 -1 반환
}

InventoryComponent::~InventoryComponent()
{
    destroy();
}

const std::vector<Item*>& InventoryComponent::getInventory() const
{
	if (items.size() > 0)
	{
		return items;
	}
    else
    {
        return std::vector<Item*>();
    }
   
}

void InventoryComponent::initialize()
{
    // �ʿ� �� ����
}

void InventoryComponent::destroy() 
{
    for (auto* item : items) {
        delete item;
    }
    items.clear();
}

void InventoryComponent::addItem(const int itemidx, int amount)
{
    int idx = FindItem(itemidx);
    if (idx != -1) 
    {
        items[idx]->addItem(amount);
    }
    else
    {
        Item* additem = ItemFactory::getInstance().createItem(itemidx, amount);
        if (additem)
        {
            items.push_back(additem);
        }
    }
}
bool InventoryComponent::reduceItem(int idx, int amount)
{
    
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



