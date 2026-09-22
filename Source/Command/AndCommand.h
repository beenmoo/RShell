#pragma once

#include "CompositeCommand.h"

class AndCommand : public CompositeCommand
{
public:
    using CompositeCommand::CompositeCommand;

    bool Execute() override;
};
