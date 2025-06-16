#include "Actor.h"

#include "Item.h"
#include "DataManager.h"
#include "GameData.h"

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
    setData(DataManager::Instance()->playerData.getData(level));
}

void Player::setData(PlayerData* data)
{
    this->data = data;
        
    baseHP = data->getHP();
    baseDamage = data->getDamage();
    baseDefense = data->getDefense();
}

void Monster::attack(Actor& target)
{
    target.damaged(*this);
}

void Monster::damaged(const Actor& attacker)
{
    int damage = attacker.GetDamage() - this->GetDefense();
    this->DecreaseHP(damage);
}
