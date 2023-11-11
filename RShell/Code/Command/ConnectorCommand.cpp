#include "ConnectorCommand.h"

void ConnectorCommand::SetLeftCommand(const std::shared_ptr<Command>& cmd)
{
    mLeftCommand = cmd;
}

void ConnectorCommand::SetRightCommand(const std::shared_ptr<Command>&cmd)
{
    mRightCommand = cmd;
}

Command* ConnectorCommand::GetLeftCommand()
{
    return mLeftCommand.get();
}

const Command* ConnectorCommand::GetLeftCommand() const
{
    return mLeftCommand.get();
}

Command* ConnectorCommand::GetRightCommand()
{
    return mRightCommand.get();
}

const Command* ConnectorCommand::GetRightCommand() const
{
    return mRightCommand.get();
}
