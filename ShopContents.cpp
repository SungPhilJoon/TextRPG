#include "Contents.h"
#include <iostream>
#include "Actor.h"
#include "DataManager.h"
#include "Manager.h"
#include "InputModule.h"
#include "GameManager.h"
#include "GameData.h"




void ShopContents::InitContents()
{
    
    *sequencer << [this](Command& command) { return this->PlayerCommandHandle(command); };
   
    
    

}

void ShopContents::EnterContents()
{
    player = GameManager::Instance()->getPlayer();
    currentState = ShopState::Menu;
    ShowContents();
}

void ShopContents::ExitContents()
{
   

}
bool ShopContents::PlayerCommandHandle(Command& command)
{
    switch (currentState)
    {
   
    case ShopState::Menu:
        return SelectContents(command);
    case ShopState::Buying:
        return HandleBuySelect(command);
    case ShopState::Selling:
        return HandleSellSelect(command);
    default:
        return true;
    }
}



void ShopContents::ShowContents()
{
    
    std::cout << "Shop Menu" << std::endl;
    std::cout << "1. BuyItem" << std::endl;
    std::cout << "2. SellItem" << std::endl;
    std::cout << "e. Quit Shop" << std::endl;
    std::cout << "q. Quit Game" << std::endl;
    std::cout << "Enter your choice:" << std::endl;
    std::cout <<"Current Gold : "<<player->getGold()<<std::endl;
}





bool ShopContents::SelectContents(Command& command)
{
  
   
    if (currentState == ShopState::Menu)
    {
       
      
        switch (command.getCommand())
        {
            case '1':
                currentState = ShopState::Buying;
                //showbuyitems(); //�ǸŸ���Ʈ 
                std::cout << "List Here" << std::endl;   // ���Ŀ� ����
                break;
            case '2':
                currentState = ShopState::Selling;
               // ShowSellItems(); //���� ����Ʈ ����
                std::cout << "List Here" << std::endl; // ���Ŀ� ����
                break;
            case 'e': //���� ������
                std::cout << "Exiting shop" << std::endl;
                return false;  // ���� ���� ��ȣ
            case 'q': 
                std::cout << "Quit Game" << std::endl;
                return false;  // ���� ���� ��ȣ
            default:
                std::cout << "Invalid input, please reinput." << std::endl;
                return true;
        }

    }
    
    return true;
}

bool ShopContents::HandleBuySelect(Command& command)
{
    if (currentState == ShopState::Buying)
    {
      
        int idx = command.getCommand() - '0' - 1;
        if (idx >= 0 && shopItems.size() > idx)
        {
      
            if (player->reduceGold(shopItems[idx]->getValue())) // ���� �ߴٸ�?
            {
               /* ItemData* data = Manager<DataManager>::Instance()->itemData.getData(idx);
                player->addItem(*(ItemFactory::CreateItem(data)));*/ 
                std::cout << "BuyItem!!!" << std::endl;

            }
            else
            {
                std::cout << "Not Enough Money" << std::endl;
               
            }
        }
        else
        {
            std::cout << "error idx" << std::endl;
           
        }
        currentState = ShopState::Menu;
        ShowContents();
       
     
  
    }
    return true;
}


bool ShopContents::HandleSellSelect(Command& command)
{
    if (currentState == ShopState::Selling)
    {
        int idx = command.getCommand() - '0' - 1;
        if (idx >= 0 && player->getInventory().size() > idx)
        {
            int addGold = player->getInventory()[idx]->getValue();
            player->reduceItem(idx);
            player->addGold(addGold);
            std::cout << "SellItem" << std::endl;
        }
        else
        {
            std::cout << "error idx" << std::endl;
           
        }
        currentState = ShopState::Menu;
        ShowContents();
    };
    return true;
}   


