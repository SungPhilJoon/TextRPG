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
    
    StringUtil::AppendStart();
    StringUtil::AppendLine("Shop Menu");
    StringUtil::AppendLine("1. BuyItem");
    StringUtil::AppendLine("2. SellItem");
    StringUtil::AppendLine("e. Quit Shop");
    StringUtil::AppendLine("q. Quit Game");
    StringUtil::AppendLine("Enter your choice:");
    StringUtil::AppendLine("Current Gold : ", player->getGold());
    StringUtil::AppendEnd();
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
        StringUtil::AppendStart();
        StringUtil::AppendLine("Exiting shop");
        StringUtil::AppendEnd();
        return false;  // ���� ���� ��ȣ
    case 'q':
        StringUtil::AppendStart();
        StringUtil::AppendLine("Quit Game");
        StringUtil::AppendEnd();
        return false;  // ���� ���� ��ȣ
    default:
        StringUtil::AppendStart();
        StringUtil::AppendLine("Invalid input, please reinput.");
        StringUtil::AppendEnd();
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
            StringUtil::AppendStart();
            StringUtil::AppendLine("BuyItem: ", shopItems[idx]->getName());
            StringUtil::AppendEnd();

        }
        else
        {
            StringUtil::AppendStart();
            StringUtil::AppendLine("Not Enough Money");
            StringUtil::AppendEnd();

        }
    }
    else
    {
        StringUtil::AppendStart();
        StringUtil::AppendLine("error idx");
        StringUtil::AppendEnd();

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
        std::string itemName = player->getInventory()[idx]->getName();
        player->reduceItem(idx);
        player->addGold(addGold);
        StringUtil::AppendStart();
        StringUtil::NewLine();
        StringUtil::AppendLine("SellItem: ", itemName);
        StringUtil::AppendEnd();
    }
    else
    {
        StringUtil::AppendStart();
        StringUtil::NewLine();
        StringUtil::AppendLine("error idx");
        StringUtil::AppendEnd();

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
