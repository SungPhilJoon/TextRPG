#include <iostream>

#include "Contents.h"
#include "Actor.h"
#include "DataManager.h"
#include "Manager.h"

void CombatContents::InitContents()
{
    monster = new Monster();
}

void CombatContents::EnterContents()
{
    MonsterData* data = Manager<DataManager>::Instance()->monsterData.getData(10001);
    monster->setData(data);
    
    std::cout << "Combat" << std::endl;

    std::cout << monster->getName() << std::endl;
}

void CombatContents::ExitContents()
{
    
}
