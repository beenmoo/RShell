#include "TestCommand.h"
#include "Utils/Logging.h"

#include <filesystem>
#include <iostream>
#include <system_error>
#include <utility>

TestCommand::TestCommand(std::string flag, std::string path)
    : mFlag(std::move(flag)),
      mPath(std::move(path))
{
}

bool TestCommand::Execute()
{
    std::error_code errorCode;
    bool result = false;

    if (mFlag == "-e")
    {
        result = std::filesystem::exists(mPath, errorCode);
    }
    else if (mFlag == "-f")
    {
        result = std::filesystem::is_regular_file(mPath, errorCode);
    }
    else if (mFlag == "-d")
    {
        result = std::filesystem::is_directory(mPath, errorCode);
    }
    else
    {
        LogError();

        return false;
    }

    std::cout << (result ? "(True)\n" : "(False)\n");

    return result;
}

void TestCommand::LogError() const
{
    LOG_ERROR("Invalid syntax.");
}
