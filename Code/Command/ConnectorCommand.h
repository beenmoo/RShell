#pragma once

#include "Command.h"
#include "Utils/Ref.h"

class ConnectorCommand : public Command
{
public:
    virtual ~ConnectorCommand() override = default;

    virtual bool Execute() = 0;

    void SetLeft(const Ref<Command>& cmd);
    void SetRight(const Ref<Command>& cmd);

    Command* GetLeft();
    const Command* GetLeft() const;
    Command* GetRight();
    const Command* GetRight() const;

private:
    Ref<Command> mLeftCommand = nullptr;
    Ref<Command> mRightCommand = nullptr;
};