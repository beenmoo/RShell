#pragma once

#include "Command.h"

#include <memory>

class CompositeCommand : public Command
{
public:
    CompositeCommand(std::shared_ptr<Command> left, std::shared_ptr<Command> right);
    ~CompositeCommand() override = default;

protected:
    Command* Left() const;
    Command* Right() const;

private:
    std::shared_ptr<Command> mLeft;
    std::shared_ptr<Command> mRight;
};
