#pragma once

#include "Command.h"

#include <memory>

class ConnectorCommand : public Command
{
public:
    virtual ~ConnectorCommand() override = default;

    virtual bool Execute() = 0;

    void SetLeftCommand(const std::shared_ptr<Command>& cmd);
    void SetRightCommand(const std::shared_ptr<Command>& cmd);

    Command* GetLeftCommand();
    const Command* GetLeftCommand() const;
    Command* GetRightCommand();
    const Command* GetRightCommand() const;

private:
    std::shared_ptr<Command> mLeftCommand = nullptr;
    std::shared_ptr<Command> mRightCommand = nullptr;
};