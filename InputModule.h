#pragma once

class Command
{
private:
    char c;

public:
    void setCommand(char c);
    void executeCommand();

    char getCommand() const;
    
};

class InputModule
{
private:
    Command* currentCommand;
    
public:
    InputModule()
    {
        currentCommand = new Command();
    }

    ~InputModule()
    {
        delete currentCommand;
    }
    
    bool Execute();

    Command* getCurrentCommand() const;
};
