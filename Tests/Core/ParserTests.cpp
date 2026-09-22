#include "Command/Command.h"
#include "Core/Lexer.h"
#include "Core/Parser.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace
{

class ParserTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mTempDir = std::filesystem::temp_directory_path() / "RShellParserTests";
        std::filesystem::create_directories(mTempDir);

        mExistingPath = mTempDir / "exists.txt";
        std::ofstream(mExistingPath) << "x";

        mMissingPath = mTempDir / "missing.txt";
    }

    void TearDown() override
    {
        std::filesystem::remove_all(mTempDir);
    }

    std::shared_ptr<Command> Parse(std::string input)
    {
        Lexer lexer;
        auto tokens = lexer.TokenizeInput(input);
        return mParser.Parse(tokens);
    }

    // Runs root and returns everything it printed - lets tests observe which TestCommand
    // leaves actually ran (and in what order) without needing to spawn any process.
    static std::string Execute(const std::shared_ptr<Command>& root)
    {
        const std::ostringstream captured;
        std::streambuf* oldBuf = std::cout.rdbuf(captured.rdbuf());

        if (root)
            root->Execute();

        std::cout.rdbuf(oldBuf);
        return captured.str();
    }

    // "test -e <existing file>" / "test -e <missing file>" stand in for portable true/false -
    // no external command needs to exist on PATH for these to work on any platform.
    std::string TrueExpr() const
    {
        return "test -e " + mExistingPath.string();
    }

    std::string FalseExpr() const
    {
        return "test -e " + mMissingPath.string();
    }

    Parser mParser;
    std::filesystem::path mTempDir;
    std::filesystem::path mExistingPath;
    std::filesystem::path mMissingPath;
};

TEST_F(ParserTest, ParsesPlainCommand)
{
    EXPECT_NE(Parse("echo hi"), nullptr);
}

TEST_F(ParserTest, EmptyInputParsesToNull)
{
    EXPECT_EQ(Parse(""), nullptr);
}

TEST_F(ParserTest, RejectsLeadingOperator)
{
    EXPECT_EQ(Parse("&& echo hi"), nullptr);
}

TEST_F(ParserTest, RejectsUnclosedParenthesis)
{
    EXPECT_EQ(Parse("( echo hi"), nullptr);
}

TEST_F(ParserTest, RejectsTrailingGarbage)
{
    EXPECT_EQ(Parse("echo hi )"), nullptr);
}

TEST_F(ParserTest, RejectsIncompleteTest)
{
    EXPECT_EQ(Parse("test -e"), nullptr);
}

TEST_F(ParserTest, RejectsUnclosedLegacyTest)
{
    EXPECT_EQ(Parse("[ -e /tmp"), nullptr);
}

TEST_F(ParserTest, ParsesLegacyTestBrackets)
{
    auto root = Parse("[ -e " + mExistingPath.string() + " ]");
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(Execute(root), "(True)\n");
}

// Regression test for the bug where the Lexer reserved "test"/"exit" as keywords everywhere,
// so "echo test" failed to parse at all - see LexerTest.KeywordsAreNotSpeciallyTokenized.
TEST_F(ParserTest, KeywordsAreOnlyRecognizedAtOperandStart)
{
    EXPECT_NE(Parse("echo test"), nullptr);
    EXPECT_NE(Parse("echo exit"), nullptr);
    EXPECT_NE(Parse("echo [ ]"), nullptr);
}

TEST_F(ParserTest, AndShortCircuitsOnFailure)
{
    auto root = Parse(FalseExpr() + " && " + TrueExpr());
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(Execute(root), "(False)\n");
}

TEST_F(ParserTest, OrShortCircuitsOnSuccess)
{
    auto root = Parse(TrueExpr() + " || " + FalseExpr());
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(Execute(root), "(True)\n");
}

TEST_F(ParserTest, SemicolonRunsBothUnconditionally)
{
    auto root = Parse(FalseExpr() + " ; " + TrueExpr());
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(Execute(root), "(False)\n(True)\n");
}

// Parentheses force a grouping that wouldn't happen by default: without them, "&&"/"||" at
// equal precedence associate left-to-right, so "true || false && false" runs as
// (true || false) && false - "true" short-circuits the "||", but its result still feeds the
// "&&", which then runs the trailing "false". Wrapping the right-hand pair forces the "||" to
// short-circuit the entire parenthesized group instead, so neither inner operand ever runs.
TEST_F(ParserTest, ParenthesesForceNonDefaultGrouping)
{
    auto withoutParens = Parse(TrueExpr() + " || " + FalseExpr() + " && " + FalseExpr());
    ASSERT_NE(withoutParens, nullptr);
    EXPECT_EQ(Execute(withoutParens), "(True)\n(False)\n");

    auto withParens = Parse(TrueExpr() + " || ( " + FalseExpr() + " && " + FalseExpr() + " )");
    ASSERT_NE(withParens, nullptr);
    EXPECT_EQ(Execute(withParens), "(True)\n");
}

TEST_F(ParserTest, NestedParenthesesParseSuccessfully)
{
    auto root = Parse("( ( " + TrueExpr() + " ) && " + TrueExpr() + " )");
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(Execute(root), "(True)\n(True)\n");
}

}  // namespace
