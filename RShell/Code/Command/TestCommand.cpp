#include "TestCommand.h"
#include "Utils/Logging.h"

#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>

TestCommand::TestCommand()
{
    SetTokenType(TokenSpec::TokenType::Test);
}

bool TestCommand::Execute()
{
    struct stat buf;

    if (GetArgumentCount() < 2)
    {
        LogError();

        return false;
    }

    auto argsIt = begin();
    std::string flag = (argsIt + 1)->GetValue();
    std::string path = (argsIt + 2)->GetValue();

    if (flag == "-e")
    {
        if (!stat(path.c_str(), &buf))
        {
            std::cout << "(True)\n";

            return true;
        }

        std::cout << "(False)\n";

        return false;
    }
    else if (flag == "-f")
    {
        if (stat(path.c_str(), &buf))
        {
            std::cout << "(False)\n";

            return false;
        }

        if (S_ISREG(buf.st_mode))
        {
            std::cout << "(True)\n";

            return true;
        }

        std::cout << "(False)\n";

        return false;
    }
    else if (flag == "-d")
    {
        if (stat(path.c_str(), &buf))
        {
            std::cout << "(False)\n";

            return false;
        }

        if (S_ISDIR(buf.st_mode))
        {
            std::cout << "(True)\n";

            return true;
        }

        std::cout << "(False)\n";

        return false;
    }
    else
        LogError();

    return false;
}

void TestCommand::LogError() const
{
    LOG_ERROR("Invalid syntax.");
}

LeftLegacyTestCommand::LeftLegacyTestCommand()
{
    SetTokenType(TokenSpec::TokenType::LeftLegacyTest);
}

bool LeftLegacyTestCommand::Execute()
{
    return false;
}

RightLegacyTestCommand::RightLegacyTestCommand()
{
    SetTokenType(TokenSpec::TokenType::RightLegacyTest);
}

bool RightLegacyTestCommand::Execute()
{
    return false;
}
