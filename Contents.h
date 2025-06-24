#pragma once

#include <iostream>
#include <vector>
#include <functional>


class ItemData;
class Command;
class Monster;
class Player;

enum class ContentsType
{
    Menu,
    Combat,
    Shop
};

class EnumeratorSequencer
{
private:
    std::vector<std::function<bool(Command&)>> sequencer;

public:
    void operator+=(std::function<bool(Command&)> func);
    void operator<<(std::function<bool(Command&)> func);
    bool ExecuteSequence(Command& command);
};

class Contents
{
protected:
    EnumeratorSequencer* sequencer;

public:
    Contents();
    ~Contents();

    virtual void InitContents() = 0;
    virtual void EnterContents() = 0;
    bool UpdateContents(Command& command);
    virtual void ExitContents() = 0;
};

class MenuContents : public Contents
{
public:
    virtual void InitContents() override;
    virtual void EnterContents() override;
    virtual void ExitContents() override;

    bool IsNickNameNotEmpty(Command& command);
    bool CanChangeContents(Command& command);
};

class CombatContents : public Contents
{
private:
    Monster* monster = nullptr;
    Player* player = nullptr;
    bool bIsBoss = false;

public:
    virtual void InitContents() override;
    virtual void EnterContents() override;
    virtual void ExitContents() override;

    bool HandlePlayerCommand(Command& command);
private:
    void PlayerAttack();
    void MonsterAttackBack();
    bool IsActorDead();
    void SetupBossMonster();
    void GameClear();   
    void GetStageClearReward();
};
enum class ShopState
{

    Menu,
    Buying,
    Selling
};
class ShopContents : public Contents
{
private:
    std::vector<ItemData*> shopItems;
    ShopState currentState = ShopState::Menu;
    Player* player;


public:
    /// <summary>
    /// index - name  - value ���̸� �����ϰڽ��ϴ� (idx = 1���� ����!)
    ///  �����۸�� �����ͼ� shopitems�� �ִ°�
    /// shopitems�� �����ִ� �������ִ� �� 
    /// �÷��̾� �κ��丮�� �����ͼ� �����ִ� �� 
    /// </summary>
    virtual void InitContents() override;
    virtual void EnterContents() override;
    virtual void ExitContents() override;
    void ShowContents();
    void ShowShopItems();
	void ShowPlayerInventoryToSell();
  // === ������ ���� �Ǹ� command ====
 
    bool PlayerCommandHandle(Command& command);
    bool SelectContents(Command& command);
    bool HandleBuySelect(Command& command);
    bool HandleSellSelect(Command& command);

};
