#pragma once

#include "ConnectorCommand.h"

class SemicolonCommand : public ConnectorCommand
{
public:
    SemicolonCommand();
    virtual ~SemicolonCommand() override = default;

    virtual bool Execute() override;
};