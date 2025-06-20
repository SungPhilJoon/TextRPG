#pragma once

class BaseManager
{
public:
    BaseManager() = default;
    virtual ~BaseManager() = default;

    virtual void InitGame() = 0;
    virtual void EnterGame() = 0;
    virtual bool UpdateGame() = 0;
    virtual void ExitGame() = 0;
};

template<typename T>
class Manager : public BaseManager
{
protected:
    static T* instance;
    
public:
    Manager() = default;
    virtual ~Manager() override = default;
    
    static T* Instance();

    virtual void InitGame() override = 0;
    virtual void EnterGame() override = 0;
    virtual bool UpdateGame() override = 0;
    virtual void ExitGame() override = 0;
};

