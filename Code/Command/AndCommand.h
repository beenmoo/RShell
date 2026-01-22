#pragma once

#include "ConnectorCommand.h"

class AndCommand : public ConnectorCommand
{
public:
    AndCommand();
    virtual ~AndCommand() override = default;

    virtual bool Execute() override;
};