#include "Manager.h"
#include "GameManager.h"
#include "DataManager.h"
#include "InputModule.h"
#include "Contents.h"
#include "Actor.h"
#include "GameData.h"

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
    contentsMap[ContentsType::Menu]->InitContents(player);
    contentsMap[ContentsType::Combat]->InitContents(player);
    contentsMap[ContentsType::Shop]->InitContents(player);

    currentContents = ContentsType::Menu;
}

void GameManager::EnterGame()
{
    while (IsNickNameEmpty())
    {
        std::string name;

        std::cout << "Please enter your nickname" << std::endl;
        std::getline(std::cin, name); // std::cin >> name;

        if (name.find(' ') != std::string::npos)
        {
            std::cout << "Please enter your nickname without space" << std::endl;
            continue;
        }

        player->SetName(name);
    }

    auto playerData = DataManager::Instance()->playerData.getData(1);
    player->setData(playerData);

    contentsMap[currentContents]->EnterContents(player);

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
        //ExitGame();
        return false;
    }

    // if (currentContents == ContentsType::Menu)
    // {
    //     ChangeContents(currentCommand);
    //     return true;
    // }

    //bool keepGoing = contentsMap[currentContents]->UpdateContents(player, currentCommand);
    if (contentsMap[currentContents]->UpdateContents(player, currentCommand))
    {
        if (IsFail() || IsClear())
        {
            return false;
        }
        
        ChangeContents(currentCommand);
    }
    
    // if (!keepGoing)
    // {
    //     Command returnMenuCommand;
    //     returnMenuCommand.setCommand('0');
    //     ChangeContents(returnMenuCommand);
    // }

    return true;

    //if (currentContents == ContentsType::Shop) // �����϶��� false�϶� menu�� �̵� 
    //{

    //    bool keepGoing = contentsMap[currentContents]->UpdateContents(currentCommand);
    //
    //    if (!keepGoing)
    //    {
    //        contentsMap[currentContents]->ExitContents();
    //        Command returnMenuCommand;
    //        returnMenuCommand.setCommand('0');
    //        ChangeContents(returnMenuCommand);
    //    }
    //    return true;
    //}

}

void GameManager::ExitGame()
{
    exit(0);
}

bool GameManager::IsNickNameEmpty()
{
    return player->IsNicknameEmpty();
}

void GameManager::ChangeContents(Command& command)
{
    contentsMap[currentContents]->ExitContents(player); // ���� ExitContents �ҷ���

    //int commandNum = command.getCommand() - '0'; // command�� ������ ����Ǿ����Ƿ�, menu�� command�� �ҷ��� 
    //currentContents = static_cast<ContentsType>(commandNum);

    std::string commandContext = "";
    commandContext += command.getCommand();

    int commandNum = std::stoi(commandContext);
    currentContents = static_cast<ContentsType>(commandNum);
    contentsMap[currentContents]->EnterContents(player);
}

void GameManager::ClearGame()
{
    isClear = true;
}

bool GameManager::IsFail()
{
    return player->IsDead();
}

bool GameManager::IsClear()
{
    return isClear; 
}