#pragma once

#include "Command.h"

class SingleCommand : public Command
{
public:
    SingleCommand();
    virtual ~SingleCommand() = default;

    virtual bool Execute() override;
};