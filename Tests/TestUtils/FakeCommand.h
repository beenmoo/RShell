#pragma once

#include "Command/Command.h"

// A Command leaf that records how many times it ran and returns a scripted result - lets
// Composite tests (And/Or/Semicolon) verify short-circuit behavior without spawning a process.
class FakeCommand : public Command
{
public:
    explicit FakeCommand(bool result)
        : mResult(result)
    {
    }

    bool Execute() override
    {
        ++mExecuteCount;
        return mResult;
    }

    int GetExecuteCount() const
    {
        return mExecuteCount;
    }

private:
    bool mResult;
    int mExecuteCount = 0;
};
