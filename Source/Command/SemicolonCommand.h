#pragma once

#include "CompositeCommand.h"

class SemicolonCommand : public CompositeCommand
{
public:
    using CompositeCommand::CompositeCommand;

    bool Execute() override;
};
