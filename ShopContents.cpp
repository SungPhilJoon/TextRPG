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
#include "ItemFactory.h"


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
				ShowShopItems();
                break;
            case '2':
                currentState = ShopState::Selling;
				ShowPlayerInventoryToSell();
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
      
        int idx = command.getCommand() - '0';
        if (idx >= 0 && shopItems.size() > idx)
        {
      
            if (player->reduceGold(shopItems[idx]->getValue())) // ���� �ߴٸ�?
            {
                player->addItem(shopItems[idx]->getIndex()); 
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
        int idx = command.getCommand() - '0';
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




void ShopContents::ShowShopItems()
{
    using namespace std;
    cout << left << setw(20) << "Item" << right << setw(10) << "Price" << endl << string(40, '-') << endl;

    for (auto item : shopItems)
    {
        cout << left << setw(20) << item->getName() << right << setw(10) << item->getValue() << right << setw(10) << "Gold" << endl <<endl ;
    }
    cout << "Button 0 : Buy Attack Booster" << endl << "Button 1 : Buy HPPostion" << endl;
}

void ShopContents::ShowPlayerInventoryToSell()
{
    using namespace std;
    int itemIndex = 0;
    cout << "Your Inventory:" << endl;
    cout << player->getInventory().size() << " items in inventory." << endl;
    for (auto item : player->getInventory())
    {
        cout << "[" << itemIndex << "] " << setw(15) << left << item->getName() << " | Count: " << setw(3) << item->getCount() << " | Price: " << item->getValue() << "Gold" << endl;
        ++itemIndex;
    }
    if (player->getInventory().empty())
    {
        std::cout << "Your inventory is empty." << std::endl;
    }
    else
    {
        std::cout << "To sell Item. Press the number of the item you want to sell." << std::endl;
    }
}