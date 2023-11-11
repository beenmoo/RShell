#pragma once

#include "ConnectorCommand.h"

class OrCommand : public ConnectorCommand
{
public:
    OrCommand();
    virtual ~OrCommand() override = default;

    virtual bool Execute() override;
};