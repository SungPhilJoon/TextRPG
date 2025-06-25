#include <iostream>

#include "Contents.h"
#include "Actor.h"
#include "DataManager.h"
#include "Manager.h"
#include "InputModule.h"
#include "GameManager.h"
#include "ItemFactory.h"

void CombatContents::InitContents()
{

    monster = new Monster();

    *sequencer << [this](Command& command) { return this->HandlePlayerCommand(command); };
}

void CombatContents::EnterContents()
{
    if (monster == nullptr)
    {
        monster = new Monster();
    }
    player = Manager<GameManager>::Instance()->getPlayer();

    int monsterID = 10001 + rand() % 10;
    MonsterData* data = Manager<DataManager>::Instance()->monsterData.getData(monsterID);
    monster->setData(data, player->GetLevel()); // 250620 플레이어 레벨 기준으로 능력치 생성위해 수정

    std::cout << monster->getName() << " appeared! HP: " << monster->GetHP() << ", Damage: " << monster->GetDamage() << std::endl;

    SetupBossMonster();
}

void CombatContents::ExitContents()
{
    if (monster)
    {
        delete monster;
        monster = nullptr;
    }
}

bool CombatContents::HandlePlayerCommand(Command& command)
{
    char cmd = command.getCommand();

    if (cmd == '1') // '1' 입력 시 자동 전투
    {
        while (true)
        {
            if (!player->getInventory().empty() && rand() % 100 < 30)
            {
                player->useItemDuringCombat();
            }

            PlayerAttack();

            if (IsActorDead()) break;

            MonsterAttackBack();

            if (IsActorDead()) break;
        }

        Command returnMenuCommand;
        returnMenuCommand.setCommand('0');
		GameManager::Instance()->ChangeContents(returnMenuCommand, true, false);
        return false;
    }
    return false;
}

void CombatContents::PlayerAttack()
{
    player->attack(*monster);
    std::cout << "You attacked " << monster->getName() << "! [Monster HP: " << monster->GetHP() << "]\n";
}

void CombatContents::MonsterAttackBack()
{
    monster->attack(*player);
    std::cout << monster->getName() << " attacks! [Player HP: " << player->GetHP() << "]\n";
}

bool CombatContents::IsActorDead()
{
    if (player->GetHP() <= 0) {
        std::cout << "You died. Game Over.\n";
        player->IncreaseHP(player->GetBaseHP());
        return true;
    }

    if (monster->GetHP() <= 0) {
        std::cout << "You defeated " << monster->getName() << "! Congratulations\n";
        if (monster->IsBoss())
        {
            GameClear();
        }
        else
        {
			GetStageClearReward();
        }

        player->addKillCount();
        return true;
    }

    return false;
}


void CombatContents::SetupBossMonster()
{
    if (player->GetLevel() < 10) return;

    monster->SetIsBoss(true);

    std::cout << "Boss " << monster->getName() << " enters the battlefield! [Boss HP: " << monster->GetBaseHP() << ", DMG: " << monster->GetDamage() << "]\n";
}

void CombatContents::GameClear()
{
    std::cout << "\nYou cleared the game! \nTotal Monsters Defeated: " << player->getMonsterKillCount() << "\n";
	std::cout << "Thank you for playing!\n";
	monster->SetIsBoss(false);
	delete monster;
	monster = nullptr;
	GameManager::Instance()->ExitGame();
}

void CombatContents::GetStageClearReward()
{
    player->GainExp(50);

    // 골드 얻기 10 ~ 20
    int goldEarned = rand() % 11 + 10;
    player->addGold(goldEarned);

    if (rand() % 100 < 30)
    {
        int itemID = 10001 + (rand() % 2)*10000;
        player->addItem(itemID);
        
        std::cout << "GetItem!!!" << "Congratulations\n";
        
    }
}