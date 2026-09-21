#pragma once

#include "Command.h"

#include <string>
#include <vector>

class SingleCommand : public Command
{
public:
    explicit SingleCommand(std::vector<std::string> args);
    ~SingleCommand() override = default;

    bool Execute() override;

private:
    std::vector<std::string> mArgs;
};
