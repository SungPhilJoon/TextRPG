#include "GameData.h"
#include "Item.h"
#include "Actor.h"

Item::~Item()
{
    delete data;
}

void Item::decreaseItem()
{
    count = std::max(0, count - 1);
}

std::string Item::getName() const
{
    return data->getName();
}

int Item::getValue() const
{
    return data->getValue();
}

void Item::addItem()
{
    ++count;
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
