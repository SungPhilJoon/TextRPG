#include "InputModule.h"

#include <cstdio>
#include <iostream>

using namespace std;

void Command::setCommand(char c)
{
    this->c = c;
}

void Command::executeCommand()
{
    cout << "Command " << c << endl;
}

char Command::getCommand() const
{
    return c;
}

bool InputModule::Execute()
{
    char c;
    while (cin >> c)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');

            continue;
        }

        if (c)
        {
            break;
        }

        return false;
    }

    currentCommand->setCommand(c);
    currentCommand->executeCommand();

    return true;
}

Command* InputModule::getCurrentCommand() const
{
    return currentCommand;
}
