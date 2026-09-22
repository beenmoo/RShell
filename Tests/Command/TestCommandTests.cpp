#include "Command/TestCommand.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{

class TestCommandTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mTempDir = std::filesystem::temp_directory_path() / "RShellTestCommandTests";
        std::filesystem::create_directories(mTempDir);

        mFilePath = mTempDir / "file.txt";
        std::ofstream(mFilePath) << "content";

        mMissingPath = mTempDir / "does-not-exist";
    }

    void TearDown() override
    {
        std::filesystem::remove_all(mTempDir);
    }

    std::filesystem::path mTempDir;
    std::filesystem::path mFilePath;
    std::filesystem::path mMissingPath;
};

TEST_F(TestCommandTest, ExistsTrueForExistingFile)
{
    TestCommand cmd("-e", mFilePath.string());
    EXPECT_TRUE(cmd.Execute());
}

TEST_F(TestCommandTest, ExistsFalseForMissingPath)
{
    TestCommand cmd("-e", mMissingPath.string());
    EXPECT_FALSE(cmd.Execute());
}

TEST_F(TestCommandTest, IsRegularFileTrueForFile)
{
    TestCommand cmd("-f", mFilePath.string());
    EXPECT_TRUE(cmd.Execute());
}

TEST_F(TestCommandTest, IsRegularFileFalseForDirectory)
{
    TestCommand cmd("-f", mTempDir.string());
    EXPECT_FALSE(cmd.Execute());
}

TEST_F(TestCommandTest, IsDirectoryTrueForDirectory)
{
    TestCommand cmd("-d", mTempDir.string());
    EXPECT_TRUE(cmd.Execute());
}

TEST_F(TestCommandTest, IsDirectoryFalseForFile)
{
    TestCommand cmd("-d", mFilePath.string());
    EXPECT_FALSE(cmd.Execute());
}

TEST_F(TestCommandTest, UnknownFlagReturnsFalse)
{
    TestCommand cmd("-x", mFilePath.string());
    EXPECT_FALSE(cmd.Execute());
}

TEST_F(TestCommandTest, PrintsTrueMarkerOnSuccess)
{
    const std::ostringstream captured;
    std::streambuf* oldBuf = std::cout.rdbuf(captured.rdbuf());

    TestCommand cmd("-e", mFilePath.string());
    cmd.Execute();

    std::cout.rdbuf(oldBuf);

    EXPECT_EQ(captured.str(), "(True)\n");
}

TEST_F(TestCommandTest, PrintsFalseMarkerOnFailure)
{
    const std::ostringstream captured;
    std::streambuf* oldBuf = std::cout.rdbuf(captured.rdbuf());

    TestCommand cmd("-e", mMissingPath.string());
    cmd.Execute();

    std::cout.rdbuf(oldBuf);

    EXPECT_EQ(captured.str(), "(False)\n");
}

}  // namespace
