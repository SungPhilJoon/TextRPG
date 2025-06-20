#include "Contents.h"
#include <iostream>
#include "Actor.h"
#include <iomanip>
#include "DataManager.h"
#include "Manager.h"
#include "InputModule.h"
#include "GameManager.h"
#include "GameData.h"
#include "Item.h"



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
        shopItems = Manager<DataManager>::Instance()->itemData.getDataContainer();
      
        switch (command.getCommand())
        {
            case '1':
                currentState = ShopState::Buying;
                
                std::cout << std::left << std::setw(20) << "Item"
                    << std::right << std::setw(10) << "Price" << std::endl;
                std::cout << std::string(40, '-') << std::endl;

                for (auto item : shopItems)
                {
                    std::cout << std::left << std::setw(20) << item->getName()
                        << std::right << std::setw(10) << item->getValue()
                        << std::right << std::setw(10) << "Gold" << std::endl;
                }
                std::cout << "Button 0 : Attack Booster" << std::endl << "Button 1 : HPPostion" << std::endl;
                break;
            case '2':
                currentState = ShopState::Selling;
				std::cout << "Your Inventory:" << std::endl;
				//player->addItem(*ItemFactory::CreateItem(shopItems[0])); // 아이템 기능 구현되면 첫 번째 아이템을 테스트로 추가 해보고 이걸로 구매 기능 구현 가능할것 같음.
          
				std::cout << player->getInventory().size() << " items in inventory." << std::endl;
				for (auto item : player->getInventory())
				{
					std::cout << std::left << std::setw(20) << item->getName()
						<< std::right << std::setw(10) << item->getCount()
					<< std::right << std::setw(10) << "Gold" << std::endl;
				}
                
                break;
            case 'e': //상점 나가기
                std::cout << "Exiting shop" << std::endl;
                return false;  // 상점 종료 신호
            case 'q': 
                std::cout << "Quit Game" << std::endl;
                return false;  // 상점 종료 신호
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
      
        int idx = command.getCommand() - '0';
        if (idx >= 0 && shopItems.size() > idx)
        {
      
            if (player->reduceGold(shopItems[idx]->getValue())) // 구매 했다면?
            {
               /* ItemData* data = Manager<DataManager>::Instance()->itemData.getData(idx);
                player->addItem(*(ItemFactory::CreateItem(data)));*/ 
                std::cout << "Buy " << shopItems[idx]->getName()<< "!!!" << std::endl;

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


