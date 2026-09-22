#pragma once

#include "Command.h"

#include <string>

class TestCommand : public Command
{
public:
    TestCommand(std::string flag, std::string path);
    ~TestCommand() override = default;

    bool Execute() override;

private:
    void ReportInvalidFlag() const;

    std::string mFlag;
    std::string mPath;
};
