#include "CompositeCommand.h"

#include <utility>

CompositeCommand::CompositeCommand(std::shared_ptr<Command> left, std::shared_ptr<Command> right)
    : mLeft(std::move(left)),
      mRight(std::move(right))
{
}

Command* CompositeCommand::Left() const
{
    return mLeft.get();
}

Command* CompositeCommand::Right() const
{
    return mRight.get();
}
