#include "Manager.h"
#include "GameManager.h"
#include "DataManager.h"
#include "InputModule.h"
#include "Contents.h"
#include "Actor.h"
#include "StringUtil.h"

template<>
GameManager* Manager<GameManager>::instance = nullptr;

template<>
GameManager* Manager<GameManager>::Instance()
{
    if (instance == nullptr)
    {
        instance = new GameManager();
    }

    return instance;
}

GameManager::GameManager()
{
    contentsMap[ContentsType::Menu] = new MenuContents();
    contentsMap[ContentsType::Combat] = new CombatContents();
    contentsMap[ContentsType::Shop] = new ShopContents();

    player = new Player();
    inputModule = new InputModule();
}

GameManager::~GameManager()
{
    for (auto contents : contentsMap)
    {
        delete contents.second;
    }

    delete player;
    delete inputModule;
}

void GameManager::InitGame()
{
    contentsMap[ContentsType::Menu]->InitContents();
    contentsMap[ContentsType::Combat]->InitContents();
    contentsMap[ContentsType::Shop]->InitContents();

    currentContents = ContentsType::Menu;
}

void GameManager::EnterGame()
{
    while (IsNickNameEmpty())
    {
        StringUtil::AppendStart();
        StringUtil::AppendLine("Welcome to the TextRPG Game!");
        StringUtil::AppendLine("Please enter your nickname (without spaces):");
        StringUtil::AppendEnd();

        std::string name;

        std::getline(std::cin, name); // std::cin >> name;

        if (name.find(' ') != std::string::npos)
        {
			StringUtil::AppendStart();
            StringUtil::AppendLine("Please enter your nickname without space");
			StringUtil::AppendEnd(0);
            continue;
        }

        player->SetName(name);
    }

    auto playerData = DataManager::Instance()->playerData.getData(1);
    player->setData(playerData);

    contentsMap[currentContents]->EnterContents();

}


bool GameManager::UpdateGame()
{
    Command currentCommand;

    if (inputModule->Execute())
    {
        currentCommand = inputModule->getCurrentCommand();
    }
    else
    {
        return true;
    }

    if (currentCommand.getCommand() == 'q')
    {
        ExitGame();
        return false;
    }

    if (currentContents == ContentsType::Menu)
    {
        if (currentCommand.getCommand() == '1')
        {
            ChangeContents(currentCommand, true, true);
        }
        else
        {
            ChangeContents(currentCommand, true, false);
        }
        return true;
    }

    bool keepGoing = contentsMap[currentContents]->UpdateContents(currentCommand);

    if (!keepGoing)
    {
        Command returnMenuCommand;
        returnMenuCommand.setCommand('0');
        ChangeContents(returnMenuCommand, true, false);
    }

    return true;

}

void GameManager::ExitGame()
{
    exit(0);
}

bool GameManager::IsNickNameEmpty()
{
    return player->IsNicknameEmpty();
}

void GameManager::ChangeContents(Command& command, bool callEnter, bool callUpdate)
{
    contentsMap[currentContents]->ExitContents(); // 전투 ExitContents 불려짐

    int commandNum = command.getCommand() - '0'; // command는 전투가 종료되었으므로, menu의 command가 불려짐 
    currentContents = static_cast<ContentsType>(commandNum);

    /*std::string commandContext = "";
    commandContext += command.getCommand();

    int commandNum = std::stoi(commandContext);
    currentContents = static_cast<ContentsType>(commandNum);*/

    if (callEnter)
    {
        contentsMap[currentContents]->EnterContents();
    }

    if (callUpdate)
    {
        contentsMap[currentContents]->UpdateContents(command);
    }
}