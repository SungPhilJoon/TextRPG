#include <iostream>

#include "Contents.h"
#include "Actor.h"
#include "DataManager.h"
#include "Manager.h"
#include "InputModule.h"
#include "GameManager.h"
#include "ItemFactory.h"
#include "StringUtil.h"
#include "GameData.h"

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

    StringUtil::AppendStart();
    StringUtil::AppendLine();
	StringUtil::AppendLine("    << ", monster->getName(), " appeared! HP: ", monster->getName(), ", Damage: ", monster->GetDamage(), " >>");
    StringUtil::AppendEnd(0);
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
    StringUtil::AppendStart();
	StringUtil::AppendLine("    You attacked ", monster->getName(), "! [Monster HP: ", monster->GetHP(), "]"); 
    StringUtil::AppendEnd(0);
}

void CombatContents::MonsterAttackBack()
{
    monster->attack(*player);
    StringUtil::AppendStart();
	StringUtil::AppendLine("    ", monster->getName(), " attacked you! [Player HP: ", player->GetHP(), "]");
    StringUtil::AppendEnd(0);
}

bool CombatContents::IsActorDead()
{
    StringUtil::AppendStart();
    if (player->GetHP() <= 0) {
        StringUtil::AppendLine("    You died. Game Over.");
        StringUtil::AppendLine();
		StringUtil::AppendEnd();
        player->IncreaseHP(player->GetBaseHP());
        return true;
    }

    if (monster->GetHP() <= 0) {
		StringUtil::AppendLine("    You defeated ", monster->getName(), "! Congratulations");
        StringUtil::AppendLine();
        StringUtil::AppendEnd();
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
    StringUtil::AppendStart();
    StringUtil::AppendLine("    Boss ", monster->getName(), " enters the battlefield! [Boss HP: ", monster->GetBaseHP(), ", DMG: ", monster->GetDamage());
    StringUtil::AppendEnd(0);

}

void CombatContents::GameClear()
{
	StringUtil::AppendStart();
    StringUtil::AppendLine("You cleared the game!");
	StringUtil::AppendLine("Total Monsters Defeated: ", player->getMonsterKillCount());
    StringUtil::AppendLine("Thank you For Playing!");
    StringUtil::AppendEnd();

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
    StringUtil::AppendStart();
    StringUtil::AppendLine("Get ",goldEarned, "Gold!!!");
	StringUtil::AppendEnd(0);

    if (rand() % 100 < 30)
    {
        int itemID = 10001 + (rand() % 2)*10000;
        player->addItem(itemID);
        
        StringUtil::AppendStart();
        StringUtil::AppendLine(Manager<DataManager>::Instance()->itemData.getData(itemID / 10000 - 1)->getName(), " Get Item!!!");
		StringUtil::AppendEnd(0);
        
    }

    StringUtil::AppendStart();
	StringUtil::AppendEnd();
}