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

void Player::useItemDuringCombat()
{
    const auto& inventory = getInventory();

    // 체력이 최대체력보다 50 이상 줄었을 때 포션 사용
    if (GetHP() <= GetBaseHP() - 50)
    {
        for (Item* item : inventory)
        {
            if (item->getType() == ItemType::Potion)
            {
                item->useItem(*this);
                std::cout << "[Item Use] Potion.\n";
                std::cout << "[Player HP]: " << GetHP() << " / " << GetBaseHP() << "\n";
                return;
            }
        }
    }
    else
    {
        for (Item* item : inventory)
        {
            if (item->getType() == ItemType::UpgradeDamage)
            {
                std::cout << "[Item Use] Damage Buff.\n";
                std::cout << "[Player ATK]: " << GetDamage() << " + 10\n";
                item->useItem(*this);
                return;
            }
        }
    }
}


void Player::levelUp()
{
    // 250620 능력치 성장 확인을 위한 이전값 저장
    int prevHP = baseHP;
    int prevDamage = baseDamage;
    int prevDefense = baseDefense;

    increaseLevel();
    setData(Manager<DataManager>::Instance()->playerData.getData(level));

    std::cout << "NOW STATUS\n";
    std::cout << "- HP: " << prevHP << " -> " << baseHP << "\n";
    std::cout << "- ATK: " << prevDamage << " -> " << baseDamage << "\n";
    std::cout << "- DEF: " << prevDefense << " -> " << baseDefense << "\n";
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

// 250620 경험치 및 레벨링
void Player::GainExp(int amount)
{
    exp += amount;
    std::cout << "\n[EXP] +" << amount << " (Current EXP: " << exp << "/100)\n";
    TryLevelUp();
}
// 250620 경험치 요구량 및 레벨 10 제한
void Player::TryLevelUp()
{
    while (exp >= 100 && level < 10)
    {
        exp -= 100;
        levelUp();

        std::cout << "[LEVEL UP] NOW " << level << " Level !!\n\n";

        IncreaseHP(baseHP); // 250622 레벨업 시 체력 회복
    }
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

// 250620 setData 함수에서 playerLevel도 넘기도록 해서 그걸 기반으로 몬스터 스탯 랜덤 생성 수정
void Monster::setData(MonsterData* data, int playerLevel)
{
    this->data = data;
    level = playerLevel;
    
    //baseHP = data->getHP();
    baseHP = rand() % (playerLevel * 11) + (playerLevel * 20);
    //baseDamage = data->getDamage();
    baseDamage = rand() % (playerLevel * 6) + (playerLevel * 5);

    baseDefense = data->getDefense();
    name = data->getName();
    currentHP = baseHP;
    incDamage = 0;
    incDefense = 0;
}

const std::string Monster::getName() const
{
    // 250620 10레벨 때 만나는 몬스터의 이름에 접두사에 boss를 붙이기 위해 수정
    // 이름을 data로부터 가져오지 않고, 실제 멤버 변수 name 참조
    return name;
}
