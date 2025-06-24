#include "GameData.h"
#include "Item.h"
#include "Actor.h"

Item::~Item()
{
    delete data;
}

void Item::decreaseItem(int amount)
{
    count = std::max(0, count - amount);
}

std::string Item::getName() const
{
    return data->getName();
}

int Item::getValue() const
{
    return data->getValue();
}

int Item::getCount() const
{
    return count;
}

const ItemData* Item::getData() const
{
    return data;
}

void Item::addItem(int amount)
{
    count += amount;
}

void UpgradeDamageItem::useItem(ItemUseable& target)
{
    target.getActor().IncreaseDamage(this->getValue());

    decreaseItem();
}

void PotionItem::useItem(ItemUseable& target)
{
    target.getActor().IncreaseHP(this->getValue());

    decreaseItem();
}

