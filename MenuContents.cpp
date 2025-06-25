#include <iostream>

#include "GameManager.h"
#include "Contents.h"
#include "InputModule.h"
#include "StringUtil.h"

void MenuContents::InitContents()
{
    *sequencer << [this](Command& cmd) { return this->IsNickNameNotEmpty(cmd); };
    *sequencer << [this](Command& cmd) { return this->CanChangeContents(cmd); };
}

void MenuContents::EnterContents()
{
	StringUtil::AppendStart();
    StringUtil::AppendLine("Game Menu");
	StringUtil::AppendLine("1. Go Combat");
	StringUtil::AppendLine("2. Go Shop");
	StringUtil::AppendLine("q. Quit Game");
	StringUtil::AppendLine("Enter your choice:");
	StringUtil::AppendEnd();
}

void MenuContents::ExitContents()
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