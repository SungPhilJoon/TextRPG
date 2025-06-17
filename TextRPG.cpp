//#include <iostream>
//#include <unordered_map>
#include "GameManager.h"
#include "DataManager.h"
#include "Contents.h"
//#include "InputModule.h"

#define Test

int main(int argc, char* argv[])
{
    std::vector<BaseManager*> managers;

    GameManager* gameManager = Manager<GameManager>::Instance();
    DataManager* dataManager = Manager<DataManager>::Instance();
    
    managers.push_back(gameManager);
    managers.push_back(dataManager);

    for (auto manager : managers)
    {
        manager->InitGame();
    }

    for (auto manager : managers)
    {
        manager->EnterGame();
    }

    bool isPlaying = true;
    while (isPlaying)
    {
        for (auto manager : managers)
        {
            isPlaying &= manager->UpdateGame();
        }
    }

    for (auto manager : managers)
    {
        manager->ExitGame();

        delete manager;
        manager = nullptr;
    }
    //
    // gameManager = Manager<GameManager>::Instance();
    // dataManager = Manager<DataManager>::Instance();
    //
    // gameManager->InitGame();
    // dataManager->InitGame();
    // dataManager->PrintData();
    //
    // gameManager->EnterGame();
    //
    // InputModule inputModule = InputModule();
    // while (true)
    // {
    //     bool isInput = inputModule.Execute();
    //     if (isInput)
    //     {
    //         std::cout << "Input" << std::endl;
    //         char commandChar = inputModule.getCurrentCommand()->getCommand();
    //         if (commandChar == 'q')
    //         {
    //             break;
    //         }
    //     }
    // }

    
    return 0;
}
