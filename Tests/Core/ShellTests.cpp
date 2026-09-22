#include "Core/Shell.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace
{

class ShellTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mTempDir = std::filesystem::temp_directory_path() / "RShellShellTests";
        std::filesystem::create_directories(mTempDir);

        mExistingPath = mTempDir / "exists.txt";
        std::ofstream(mExistingPath) << "x";

        mMissingPath = mTempDir / "missing.txt";

        mOldCinBuf = std::cin.rdbuf();
        mOldCoutBuf = std::cout.rdbuf(mCapturedOutput.rdbuf());
    }

    void TearDown() override
    {
        std::cin.rdbuf(mOldCinBuf);
        std::cout.rdbuf(mOldCoutBuf);
        std::filesystem::remove_all(mTempDir);
    }

    // Feeds `input` to a fresh Shell as if it were piped stdin, and returns Run()'s result.
    static bool RunWithInput(const std::string& input)
    {
        const std::istringstream inputStream(input);
        std::cin.rdbuf(inputStream.rdbuf());

        Shell shell;
        return shell.Run();
    }

    std::string TrueExpr() const
    {
        return "test -e " + mExistingPath.string();
    }

    std::string FalseExpr() const
    {
        return "test -e " + mMissingPath.string();
    }

    std::ostringstream mCapturedOutput;
    std::filesystem::path mTempDir;
    std::filesystem::path mExistingPath;
    std::filesystem::path mMissingPath;
    std::streambuf* mOldCinBuf = nullptr;
    std::streambuf* mOldCoutBuf = nullptr;
};

// Regression test: GetInput() used to ignore std::getline's failure on EOF, so once stdin ran
// out without an explicit "exit" the Update() loop never terminated - it just spun printing the
// prompt forever. Run() must return once input is exhausted, not hang.
TEST_F(ShellTest, ReturnsInsteadOfHangingWhenInputReachesEOF)
{
    EXPECT_TRUE(RunWithInput(TrueExpr() + "\n"));
}

TEST_F(ShellTest, RunReturnsLastCommandsResult)
{
    EXPECT_TRUE(RunWithInput(TrueExpr() + "\n"));
    EXPECT_FALSE(RunWithInput(FalseExpr() + "\n"));
}

TEST_F(ShellTest, LastResultIgnoresTrailingBlankLines)
{
    EXPECT_FALSE(RunWithInput(FalseExpr() + "\n\n\n"));
}

TEST_F(ShellTest, LastResultIgnoresTrailingSyntaxErrors)
{
    EXPECT_FALSE(RunWithInput(FalseExpr() + "\n&& stray\n"));
}

TEST_F(ShellTest, DefaultsToSuccessWhenNoCommandEverRan)
{
    EXPECT_TRUE(RunWithInput("\n\n"));
}

}  // namespace
