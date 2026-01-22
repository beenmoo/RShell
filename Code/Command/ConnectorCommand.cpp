#include "ConnectorCommand.h"

void ConnectorCommand::SetLeft(const Ref<Command>& cmd)
{
    mLeftCommand = cmd;
}

void ConnectorCommand::SetRight(const Ref<Command>& cmd)
{
    mRightCommand = cmd;
}

Command* ConnectorCommand::GetLeft()
{
    return mLeftCommand.get();
}

const Command* ConnectorCommand::GetLeft() const
{
    return mLeftCommand.get();
}

Command* ConnectorCommand::GetRight()
{
    return mRightCommand.get();
}

const Command* ConnectorCommand::GetRight() const
{
    return mRightCommand.get();
}
