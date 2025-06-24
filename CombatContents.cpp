#include <iostream>

#include "Contents.h"
#include "Actor.h"
#include "DataManager.h"
#include "GameData.h"
#include "Manager.h"
#include "InputModule.h"
#include "GameManager.h"

void CombatContents::InitContents(Player* player)
{
    *sequencer << [this, player](Command& command) { return this->HandlePlayerCommand(player, command); };
    *sequencer << [this, player](Command& command) { return this->HandleMonster(player); };
    *sequencer << [this, player](Command& command) { return this->HandleNextContents(player,command); };
}

void CombatContents::EnterContents(Player* player)
{
    if (monster == nullptr)
    {
        monster = new Monster();
    }

    monster->setData(player->GetLevel()); // 250620 �÷��̾� ���� �������� �ɷ�ġ �������� ����

    std::cout << monster->getName() << " enters the battlefield! [HP: " << monster->GetBaseHP() << ", DMG: " << monster->GetDamage() << "]\n";
}

void CombatContents::ExitContents(Player* player)
{
    if (monster)
    {
        delete monster;
        monster = nullptr;
    }
}

bool CombatContents::HandlePlayerCommand(Player* player, Command& command)
{
    if (monster->IsDead())
    {
        return true;
    }
    
    char cmd = command.getCommand();

    if (player->GetHP() <= player->GetBaseHP() - 50)
    {
        player->useItemDuringCombat();
    }
    
    if (cmd == '1') // '1' �Է� �� �ڵ� ����
    {
        player->attack(*monster);
        std::cout << "You attacked " << monster->getName() << "! [Monster HP: " << monster->GetHP() << "]\n";
        if (IsMonsterDead())
        {
            std::cout << "You defeated " << monster->getName() << "! Congratulations\n";
            if (IsLastStage(player->GetLevel()))
            {
                GameClear(player);
            }
            else
            {
                GetStageClearReward(player);
            }
        }

        return true;
    }
    
    return false;
}

bool CombatContents::HandleMonster(Player* player)
{
    if (monster->IsDead())
    {
        return true;
    }
    
    monster->attack(*player);
    if (IsPlayerDead(player))
    {
        //ExitContents(player);

        //GameManager::Instance()->ExitGame();

        std::cout << "You died. Game Over.\n";
        
        return true;
    }

    return false;
}

bool CombatContents::HandleNextContents(Player* player, Command& command)
{
    if (IsPlayerDead(player) || IsLastStage(player->GetLevel()))
    {
        return true;
    }
    
    if (command.getCommand() == '0')
    {
        return true;
    }

    return false;
}

bool CombatContents::IsPlayerDead(Player* player)
{
    if (player->IsDead() == false)
    {
        return false;
    }

    return true;
}

bool CombatContents::IsMonsterDead()
{
    if (monster->IsDead() == false)
    {
        return false;
    }

    return true;
}

void CombatContents::GameClear(Player* player)
{
    std::cout << "\nYou cleared the game! \nTotal Monsters Defeated: " << player->getMonsterKillCount() << "\n";
	std::cout << "Thank you for playing!\n";

    //ExitContents(player);

    GameManager::Instance()->ClearGame();
    
	//GameManager::Instance()->ExitGame();
}

void CombatContents::GetStageClearReward(Player* player)
{
    player->GainExp(monster->getExp());

    // ��� ��� 10 ~ 20
    int goldEarned = rand() % 11 + 10;
    player->addGold(goldEarned);

    /*
    if (rand() % 100 < 30)
    {
        int itemID = 10001 + rand() % 2;
        ItemData* data = Manager<DataManager>::Instance()->itemData.getData(itemID);
        Item* rewardItem = ItemFactory::CreateItem(data); ����
        player->addItem(*rewardItem);
        std::cout << "GetItem!!!" << "Congratulations\n";
    }*/
}

bool CombatContents::IsLastStage(int playerLevel)
{
    return PlayerData::IsMaxLevel(playerLevel) && monster->getType() == MonsterData::Type::Boss;
}
