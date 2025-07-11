#include "Actor.h"
#include "Item.h"
#include "DataManager.h"
#include "GameData.h"
#include "Component.h"
#include "StringUtil.h"

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
    int damage = attacker.GetDamage();
    this->DecreaseHP(damage);
}

void Player::useItem(Item& item)
{
    item.useItem(*this);
}

void Player::useItemDuringCombat()
{
    const auto& inventory = getInventory();

    for (Item* item : inventory)
    {
        if (item->getType() == ItemType::Potion && item->getCount() > 0)
        {
            item->useItem(*this);

            StringUtil::AppendStart();
            StringUtil::AppendLine("    [Item Used] Healing Potion.");
            StringUtil::AppendLine();
            StringUtil::AppendLine("    [Player HP]: ", GetHP(), " / ", GetBaseHP());
            StringUtil::AppendEnd(0);
            return;
        }

        if (item->getType() == ItemType::UpgradeDamage && item->getCount() > 0)
        {
            StringUtil::AppendStart();
            StringUtil::AppendLine("    [Item Used] Permanent Damage Buff Item.");
            StringUtil::AppendLine();
            item->useItem(*this);
            StringUtil::AppendLine("    [Player ATK]: ", GetBaseDamage(), GetIncDamageString());
            StringUtil::AppendEnd(0);
            return;
        }
    }
}


void Player::levelUp()
{
    // 250620 �ɷ�ġ ���� Ȯ���� ���� ������ ����
    int prevHP = baseHP;
    int prevDamage = baseDamage;
    int prevDefense = baseDefense;

    if (level >= 10)
    {
        std::cerr << "[LEVEL UP BLOCKED] Maximul level (10) reached.\n"; //예외
        return;
    }

    increaseLevel();

    if (level > 10)
    {
        std::cerr << "[Warning] Abnormal level detected: " << level << ". Resetting to 10. \n"; //예외
        level = 10;
        return;
    }
    setData(Manager<DataManager>::Instance()->playerData.getData(level));

    StringUtil::AppendStart();
    StringUtil::AppendLine("LEVEL UP!!! NOW STATUS: ");
    StringUtil::AppendLine("- HP: ", prevHP, " -> ", baseHP);
    StringUtil::AppendLine("- ATK: ", prevDamage, GetIncDamageString(),  " -> ", baseDamage, GetIncDamageString());
    StringUtil::AppendLine("- DEF: ", prevDefense, " -> ", baseDefense);
    StringUtil::AppendEnd();
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
    StringUtil::AppendStart();
    StringUtil::AppendLine("Inventory is empty.");
    StringUtil::AppendEnd();
    throw std::runtime_error("Inventory is null");
}

bool Player::reduceItem(int idx, int amount)
{
    return inventory->reduceItem(idx, amount);
}

void Player::addItem(int itemidx, int amount)
{
    return inventory->addItem(itemidx,amount);
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

	if (!bIsInitialized)
	{
        StringUtil::AppendStart();
        StringUtil::AppendLine();
        StringUtil::AppendLine("Hello Player: ", name);
        StringUtil::AppendLine("HP: ", baseHP);
        StringUtil::AppendLine("ATK: ", baseDamage, GetIncDamageString());
        StringUtil::AppendLine("Level: ", level);
        StringUtil::AppendLine();
        StringUtil::AppendEnd();
		bIsInitialized = true;
	}
}

// 250620 ����ġ �� ������
void Player::GainExp(int amount)
{
    exp += amount;
    StringUtil::AppendStart();
    StringUtil::AppendLine("[EXP] + ", amount, " (Current EXP: ", exp, "/100)");
    StringUtil::AppendEnd(0);
    TryLevelUp();
}
// 250620 ����ġ �䱸�� �� ���� 10 ����
void Player::TryLevelUp()
{
    while (exp >= 100 && level < 10)
    {
        exp -= 100;
        levelUp();

        StringUtil::AppendStart();
        StringUtil::Append();
        StringUtil::AppendLine("[LEVEL UP] NEW ", level, " Level !!");
        StringUtil::AppendEnd();

        IncreaseHP(baseHP);
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

// 250620 setData �Լ����� playerLevel�� �ѱ⵵�� �ؼ� �װ� ������� ���� ���� ���� ���� ����
void Monster::setData(MonsterData* data, int playerLevel)
{
    this->data = data;
    level = playerLevel;

    baseHP = rand() % (playerLevel * 11) + (playerLevel * 20);
    baseDamage = rand() % (playerLevel * 6) + (playerLevel * 5);

    if (level >= 10)
    {
        float multiplier = 1.5f + rand() % 51 / 100.0f;  // 1.5 ~ 2.0
        baseHP = static_cast<int>(baseHP * multiplier);
        baseDamage = static_cast<int>(baseDamage * multiplier);
    }

    baseDefense = data->getDefense();
    name = data->getName();
    currentHP = baseHP;
    incDamage = 0;
    incDefense = 0;
}

const std::string Monster::getName() const
{
    // 250620 10���� �� ������ ������ �̸��� ���λ翡 boss�� ���̱� ���� ����
    // �̸��� data�κ��� �������� �ʰ�, ���� ��� ���� name ����
    return name;
}
