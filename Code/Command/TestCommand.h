#pragma once

#include "ConnectorCommand.h"

class TestCommand : public ConnectorCommand
{
public:
    TestCommand();
    virtual ~TestCommand() override = default;

    virtual bool Execute() override;

private:
    void LogError() const;
};

class LeftLegacyTestCommand : public ConnectorCommand
{
public:
    LeftLegacyTestCommand();
    virtual ~LeftLegacyTestCommand() override = default;

    virtual bool Execute() override;
};

class RightLegacyTestCommand : public ConnectorCommand
{
public:
    RightLegacyTestCommand();
    virtual ~RightLegacyTestCommand() override = default;

    virtual bool Execute() override;
};