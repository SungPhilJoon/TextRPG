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
    /// index - name  - value 순이면 감사하겠습니다 (idx = 1부터 시작!)
    ///  아이템목록 가져와서 shopitems에 넣는것
    /// shopitems를 보여주는 나열해주는 것 
    /// 플레이어 인벤토리를 가져와서 보여주는 것 
    /// </summary>
    virtual void InitContents() override;
    virtual void EnterContents() override;
    virtual void ExitContents() override;
    void ShowContents();
    void ShowShopItems();
	void ShowPlayerInventoryToSell();
  // === 아이템 구매 판매 command ====
 
    bool PlayerCommandHandle(Command& command);
    bool SelectContents(Command& command);
    bool HandleBuySelect(Command& command);
    bool HandleSellSelect(Command& command);

};
