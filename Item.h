#pragma once
#include <string>

class Actor;
class ItemUseable;
class ItemData;

enum class ItemType
{
    Potion,
    UpgradeDamage,
    None
};

class Item
{
protected:
    ItemData* data;
    int count;
    ItemType type = ItemType::None;

public:
    ~Item();

    std::string getName() const;

    int getValue() const;

    int getCount() const;
    const ItemData* getData() const;

    void addItem(int amount = 1);

    void decreaseItem(int amount =1);

    ItemType getType() const { return type; }

    virtual void useItem(ItemUseable&) = 0;

    virtual Item* clone() const = 0;

    
};

class UpgradeDamageItem : public Item
{
public:
    UpgradeDamageItem() { type = ItemType::UpgradeDamage; count = 1; }
    UpgradeDamageItem(ItemData* itemData, int cnt = 1) { type = ItemType::UpgradeDamage; count = cnt; data = itemData; }
    virtual void useItem(ItemUseable&) override;
    virtual Item* clone() const override { return new UpgradeDamageItem(*this);}
};

class PotionItem : public Item
{
public:
    PotionItem() { type = ItemType::Potion; count = 1;}
    PotionItem(ItemData* itemData, int cnt = 1)
    {
        type = ItemType::Potion; count = cnt; data = itemData;
    }
    virtual void useItem(ItemUseable&) override;
    virtual Item* clone() const override{ return new PotionItem(*this);}
};

class ItemUseable
{
private:
    Actor* owner = nullptr;

protected:
    ItemUseable(Actor* actor) : owner(actor) {}

public:
    Actor& getActor()
    {
        return *owner;
    }

    virtual void useItem(Item&) = 0;
};

