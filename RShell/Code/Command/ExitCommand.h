#pragma once

#include "Command.h"

class ExitCommand : public Command
{
public:
    ExitCommand();
    virtual ~ExitCommand() = default;

    virtual bool Execute() override;
};