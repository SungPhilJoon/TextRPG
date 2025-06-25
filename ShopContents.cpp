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
#include "StringUtil.h"



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

std::string PadLeft(const std::string& str, size_t width, char space = ' ')
{
    if (str.length() >= width) return str;
    return str + std::string(width - str.length(), space);
}

std::string PadRight(const std::string& str, size_t width, char space = ' ')
{
    if (str.length() >= width) return str;
    return std::string(width - str.length(), space) + str;
}

void ShopContents::ShowShopItems()
{
    using namespace std;
    string Line = string(40, '-');
    StringUtil::AppendStart();
    StringUtil::AppendLine();
    StringUtil::AppendLine(PadLeft("Item", 20), PadLeft("| Price", 10));
    StringUtil::AppendLine(Line);

    for (auto item : shopItems)
    {
        StringUtil::AppendLine(PadLeft(item->getName(), 20), "| ", item->getValue(), "  Gold");
        StringUtil::NewLine();
    }
    StringUtil::AppendLine(Line);
    StringUtil::AppendLine("Button [0] : Buy Attack Booster");
    StringUtil::AppendLine("Button [1] : Buy HPPostion");
    StringUtil::NewLine();
    StringUtil::AppendEnd();
}

void ShopContents::ShowPlayerInventoryToSell()
{
    using namespace std;
    string Line = string(40, '-');
    int itemIndex = 0;
    StringUtil::AppendStart();
    StringUtil::NewLine();
    StringUtil::AppendLine(player->getInventory().size(), " items in inventory.");
    StringUtil::AppendLine(Line);
    for (auto item : player->getInventory())
    {
        StringUtil::AppendLine("[", itemIndex, "] ", PadLeft(item->getName(), 15), "I count:", PadLeft(to_string(item->getCount()), 3), "I Price:", item->getValue() * 6 / 10, "  Gold");
        StringUtil::NewLine();
        ++itemIndex;
    }
    if (player->getInventory().empty())
    {
        StringUtil::AppendLine("Your inventory is empty.");
    }
    else
    {
        StringUtil::NewLine();
        StringUtil::AppendLine("To sell Item. Press the number of the item you want to sell.");
        StringUtil::NewLine();
    }
    StringUtil::AppendEnd();
}
