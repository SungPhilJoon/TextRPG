#include <iostream>

#include "DataManager.h"
#include "InputModule.h"

#define Test

class GameManager
{
public:
    void Initialize()
    {
        
    }
};

DataManager* dataManager = nullptr;

int main(int argc, char* argv[])
{
    dataManager = DataManager::Instance();

    dataManager->Initialize();
    dataManager->PrintData();
    
    InputModule inputModule = InputModule();
    while (true)
    {
        bool isInput = inputModule.Execute();
        if (isInput)
        {
            std::cout << "Input" << std::endl;
            char commandChar = inputModule.getCurrentCommand()->getCommand();
            if (commandChar == 'q')
            {
                break;
            }
        }
    }

    
    return 0;
}
