#pragma once
#include <string>

class Actor;
class ItemUseable;
class ItemData;

class Item
{
protected:
    ItemData* data;
    int count;

    void decreaseItem();
    
public:
    ~Item();

    std::string getName() const;

    int getValue() const;

    void addItem();
    
    virtual void useItem(ItemUseable&) = 0;
};

class UpgradeDamageItem : public Item
{
public:
    virtual void useItem(ItemUseable&) override;
};

class PotionItem : public Item
{
public:
    virtual void useItem(ItemUseable&) override;
};

class ItemUseable
{
private:
    Actor* owner = nullptr;
    
protected:
    ItemUseable(Actor* actor) : owner(actor) { }
    
public:
    Actor& getActor()
    {
        return *owner;
    }
    
    virtual void useItem(Item&) = 0;
};
