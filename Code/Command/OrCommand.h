#pragma once

#include "CompositeCommand.h"

class OrCommand : public CompositeCommand
{
public:
    using CompositeCommand::CompositeCommand;

    bool Execute() override;
};
