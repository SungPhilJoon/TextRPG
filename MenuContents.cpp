#include <iostream>

#include "GameManager.h"
#include "Contents.h"
#include "InputModule.h"

void MenuContents::InitContents(Player* player)
{
    *sequencer << [this](Command& cmd) { return this->IsNickNameNotEmpty(cmd); };
    *sequencer << [this](Command& cmd) { return this->CanChangeContents(cmd); };
}

void MenuContents::EnterContents(Player* player)
{
    //std::system("cls");
    
    std::cout << "Game Menu" << std::endl;
    std::cout << "1. Go Combat" << std::endl;
    std::cout << "2. Go Shop" << std::endl;
	std::cout << "q. Quit Game" << std::endl;
    std::cout << "Enter your choice:" << std::endl;
}

void MenuContents::ExitContents(Player* player)
{
    
}

bool MenuContents::IsNickNameNotEmpty(Command& command)
{
    return !GameManager::Instance()->IsNickNameEmpty();
}

bool MenuContents::CanChangeContents(Command& command)
{
    char commandContext = command.getCommand();
    if (commandContext == '1' || commandContext == '2')
    {
        return true;
    }

    return false;
}