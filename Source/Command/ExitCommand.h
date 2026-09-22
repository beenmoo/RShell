#pragma once

#include "Command.h"

class ExitCommand : public Command
{
public:
    ~ExitCommand() override = default;

    bool Execute() override;
};
