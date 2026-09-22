#include "Platform/Process.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace
{

TEST(ProcessTest, EmptyArgsReturnsFalse)
{
    EXPECT_FALSE(Platform::RunProcess({}));
}

TEST(ProcessTest, RunsSuccessfully)
{
#ifdef _WIN32
    const std::vector<std::string> args = {"where", "where"};
#else
    const std::vector<std::string> args = {"true"};
#endif

    EXPECT_TRUE(Platform::RunProcess(args));
}

TEST(ProcessTest, ReturnsFalseOnNonZeroExit)
{
#ifdef _WIN32
    const std::vector<std::string> args = {"where", "this-does-not-exist-xyz"};
#else
    const std::vector<std::string> args = {"false"};
#endif

    EXPECT_FALSE(Platform::RunProcess(args));
}

TEST(ProcessTest, UnknownCommandReturnsFalse)
{
    EXPECT_FALSE(Platform::RunProcess({"this-command-definitely-does-not-exist-xyz-123"}));
}

}  // namespace
