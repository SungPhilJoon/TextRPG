#include "Actor.h"

#include "Item.h"
#include "DataManager.h"
#include "GameData.h"
#include "Component.h"
Player::Player() : ItemUseable(this)
, inventory(new InventoryComponent())
, wallet(new CurrencyComponent())
{
    inventory->setOwner(this);
    wallet->setOwner(this);
}
void Player::attack(Actor& target)
{
    target.damaged(*this);
}

void Player::damaged(const Actor& attacker)
{
    int damage = attacker.GetDamage() - this->GetDefense();
    this->DecreaseHP(damage);
}

void Player::useItem(Item& item)
{
    item.useItem(*this);
}


void Player::levelUp()
{
    increaseLevel();
    setData(Manager<DataManager>::Instance()->playerData.getData(level));
}

bool Player::IsNicknameEmpty()
{
    return name.empty();
}

const std::vector<Item*>& Player::getInventory() const
{
    if (inventory)
    {
        return inventory->getInventory();
    }
    std::cout << "Inventory is empty." << std::endl;
    throw std::runtime_error("Inventory is null");
}

bool Player::reduceItem(int idx, int amount)
{
    return inventory->reduceItem(idx, amount);
}

void Player::addItem(const Item& item)
{
    return inventory->addItem(item);
}



int Player::getGold() const
{
    return wallet->getGold();
}

void Player::addGold(int amount)
{
    wallet->addGold(amount);
}

bool Player::reduceGold(int amount)
{
    return wallet->reduceGold(amount);
}

void Player::setData(PlayerData* data)
{
    this->data = data;

    level = data->getLevel();
    
    baseHP = data->getHP();
    baseDamage = data->getDamage();
    baseDefense = data->getDefense();

    currentHP = baseHP;

    int prevIncDamage = incDamage;
    int prevIncDefense = incDefense;

    incDamage = prevIncDamage;
    incDefense = prevIncDefense;
}

void Monster::attack(Actor& target)
{
    target.damaged(*this);
}

void Monster::damaged(const Actor& attacker)
{
    int damage = attacker.GetDamage();
        //- this->GetDefense();
    this->DecreaseHP(damage);

}

void Monster::setData(MonsterData* data)
{
    this->data = data;

    level = data->getLevel();
    
    //baseHP = data->getHP();
    baseHP = rand() % (level * 11) + (level * 20);
    //baseDamage = data->getDamage();
    baseDamage = rand() % (level * 6) + (level * 5);

    baseDefense = data->getDefense();

    name = data->getName();

    currentHP = baseHP;

    incDamage = 0;
    incDefense = 0;
}

const std::string Monster::getName() const
{
    return data->getName();
}
