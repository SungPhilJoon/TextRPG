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
   
   
    player = GameManager::Instance()->getPlayer();

    *sequencer << [this](Command& command) { return currentHandler(command); };

}

void ShopContents::EnterContents()
{
    ShowMenu();
    BindSelectHandler();
}

void ShopContents::ExitContents()
{

}

void ShopContents::BindSelectHandler()
{
    currentHandler = std::bind(&ShopContents::HandleSelectContents, this, std::placeholders::_1);
}

void ShopContents::BindSellHandler()
{
    currentHandler = std::bind(&ShopContents::HandleSellSelect, this, std::placeholders::_1);
}

void ShopContents::BindBuyHandler()
{
    currentHandler = std::bind(&ShopContents::HandleBuySelect, this, std::placeholders::_1);
}

void ShopContents::ShowMenu()
{
    
    std::cout << "Shop Menu" << std::endl;
    std::cout << "1. BuyItem" << std::endl;
    std::cout << "2. SellItem" << std::endl;
    std::cout << "e. Quit Shop" << std::endl;
    std::cout << "q. Quit Game" << std::endl;
    std::cout << "Enter your choice:" << std::endl;
    std::cout <<"Current Gold : "<<player->getGold()<<std::endl;
}

bool ShopContents::HandleSelectContents(Command& command)
{
    shopItems = Manager<DataManager>::Instance()->itemData.getDataContainer();
    switch (command.getCommand())
    {
    case '1':
        BindBuyHandler();
        ShowShopItems();
        break;
    case '2':
        BindSellHandler();
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
    return true;
}

bool ShopContents::HandleBuySelect(Command& command)
{
    int idx = command.getCommand() - '0';
    if (idx >= 0 && shopItems.size() > idx)
    {

        if (player->reduceGold(shopItems[idx]->getValue())) // ���� �ߴٸ�?
        {
            player->addItem(shopItems[idx]->getIndex());
            std::cout << "Buy " << shopItems[idx]->getName() << "!!!" << std::endl;

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
    BindSelectHandler();
    ShowMenu();      
    return true;
}

bool ShopContents::HandleSellSelect(Command& command)
{
    int idx = command.getCommand() - '0';
    if (idx >= 0 && player->getInventory().size() > idx)
    {
        int addGold = player->getInventory()[idx]->getValue()*6/10;
        player->reduceItem(idx);
        player->addGold(addGold);
        std::cout << "SellItem" << std::endl;
    }
    else
    {
        std::cout << "error idx" << std::endl;

    }
    BindSelectHandler();
    ShowMenu();
    return true;
}   

void ShopContents::ShowShopItems()
{
    using namespace std;
	cout << endl << "Shop Items:" << endl;
    cout << left << setw(20) << "Item" << right << setw(10) << "Price" << endl << string(40, '-') << endl;

    for (auto item : shopItems)
    {
        cout << left << setw(20) << item->getName() << right << setw(10) << item->getValue() << right << setw(10) << "Gold" << endl <<endl ;
    }
    cout << "Button [0] : Buy Attack Booster" << endl << "Button [1] : Buy HPPostion" << endl <<endl;
}

void ShopContents::ShowPlayerInventoryToSell()
{
    using namespace std;
    int itemIndex = 0;
    cout << endl;
    cout << player->getInventory().size() << " items in inventory." << endl <<endl;
    for (auto item : player->getInventory())
    {
        cout << "[" << itemIndex << "] " << setw(15) << left << item->getName() << " | Count: " << setw(3) << item->getCount() << " | Price: " << item->getValue() << "Gold" << endl <<endl;
        ++itemIndex;
    }
    if (player->getInventory().empty())
    {
        std::cout << "Your inventory is empty." << std::endl;
    }
    else
    {
        std::cout << "To sell Item. Press the number of the item you want to sell." << std::endl <<endl;
    }
}