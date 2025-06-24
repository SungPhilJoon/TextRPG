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

    virtual void InitContents(Player*) = 0;
    virtual void EnterContents(Player*) = 0;
    bool UpdateContents(Player*, Command&);
    virtual void ExitContents(Player*) = 0;
};

class MenuContents : public Contents
{
public:
    virtual void InitContents(Player*) override;
    virtual void EnterContents(Player*) override;
    virtual void ExitContents(Player*) override;

    bool IsNickNameNotEmpty(Command& command);
    bool CanChangeContents(Command& command);
};

class CombatContents : public Contents
{
private:
    Monster* monster = nullptr;
    bool bIsBoss = false;

    bool IsLastStage(int);
    
public:
    virtual void InitContents(Player*) override;
    virtual void EnterContents(Player*) override;
    virtual void ExitContents(Player*) override;

    bool HandlePlayerCommand(Player* player, Command& command);
    bool HandleMonster(Player*);
    bool HandleNextContents(Player*, Command&);
private:
    bool IsPlayerDead(Player*);
    bool IsMonsterDead();
    void GameClear(Player*);   
    void GetStageClearReward(Player*);
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
    virtual void InitContents(Player*) override;
    virtual void EnterContents(Player*) override;
    virtual void ExitContents(Player*) override;
    void ShowContents();

  // === ������ ���� �Ǹ� command ====
 
    bool PlayerCommandHandle(Command& command);
    bool SelectContents(Command& command);
    bool HandleBuySelect(Command& command);
    bool HandleSellSelect(Command& command);

};
