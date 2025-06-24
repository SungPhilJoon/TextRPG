#include "Contents.h"

void EnumeratorSequencer::operator+=(std::function<bool(Command&)> func)
{
    sequencer.emplace_back(func);
}

void EnumeratorSequencer::operator<<(std::function<bool(Command&)> func)
{
    operator+=(func);
}


bool EnumeratorSequencer::ExecuteSequence(Command& command)
{
    for (int i = 0; i < sequencer.size(); i++)
    {
        if (!sequencer[i](command))
        {
            return false;
        }
    }

    return true;
}

Contents::Contents() : sequencer(new EnumeratorSequencer()) {}

Contents::~Contents()
{
    delete sequencer;
}

bool Contents::UpdateContents(Player* player, Command& command)
{
    return sequencer->ExecuteSequence(command);
}


