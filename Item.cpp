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

Item* ItemFactory::CreateItem(ItemData* data)
{
    int  num = (data->getIndex()%10);
    switch (num)
    {
    case 1:
        return new UpgradeDamageItem(data);
    case 2:
        return new PotionItem(data);

    default:
        return nullptr;
    }
    return nullptr;
}
