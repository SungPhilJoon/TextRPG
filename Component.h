#pragma once

#include <vector>

class player;
class Actor;
class Item;

class Component
{
protected:
    Actor* owner = nullptr;

public:
    virtual ~Component() = default;
    virtual void initialize();  
    virtual void destroy();

    Actor* getOwner() const { return owner; }
    void setOwner(Actor* own) { owner = own; }
};


class CurrencyComponent : public Component 
{
private:
    int gold = 100;

public:
    int getGold() const;
    void initialize() override;
    void addGold(int amount);
    bool reduceGold(int amount);
};

class InventoryComponent : public Component 
{
private:
    std::vector<Item*> items;
    int FindItem(const Item& target) const;
    int FindItem(const int idx) const;

public:
    ~InventoryComponent();
    const std::vector<Item*>& getInventory() const;
    void initialize() override;
    void destroy() override;
    void addItem(const int itemidx, int amount);
    bool reduceItem(int idx, int amount);
 
};
