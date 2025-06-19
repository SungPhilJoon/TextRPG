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
    for (auto& func : sequencer)
    {
        if (!func(command))
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

bool Contents::UpdateContents(Command& command)
{
    return sequencer->ExecuteSequence(command);
}


