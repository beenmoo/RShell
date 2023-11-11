#pragma once

#include "SingleCommand.h"

class LeftParenthesisCommand : public SingleCommand
{
public:
    LeftParenthesisCommand();
    virtual ~LeftParenthesisCommand() override = default;

    virtual bool Execute() override;
};

class RightParenthesisCommand : public SingleCommand
{
public:
    RightParenthesisCommand();
    virtual ~RightParenthesisCommand() override = default;

    virtual bool Execute() override;
};