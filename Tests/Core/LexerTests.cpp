#include "Core/Lexer.h"

#include <gtest/gtest.h>

#include <string>

namespace
{

TEST(LexerTest, ClassifiesOperatorsAndBrackets)
{
    Lexer lexer;
    std::string input = "&& || ; ( )";
    auto tokens = lexer.TokenizeInput(input);

    ASSERT_EQ(tokens.size(), 5u);
    EXPECT_EQ(tokens[0].GetSpec().GetTokenType(), TokenSpec::TokenType::And);
    EXPECT_EQ(tokens[1].GetSpec().GetTokenType(), TokenSpec::TokenType::Or);
    EXPECT_EQ(tokens[2].GetSpec().GetTokenType(), TokenSpec::TokenType::Semicolon);
    EXPECT_EQ(tokens[3].GetSpec().GetTokenType(), TokenSpec::TokenType::LeftParenthesis);
    EXPECT_EQ(tokens[4].GetSpec().GetTokenType(), TokenSpec::TokenType::RightParenthesis);
}

// Regression test: "test"/"exit"/"["/"]" must NOT get their own TokenType from the Lexer, since
// it has no grammatical context to tell "echo test" (a plain argument) apart from "test -e x"
// (the builtin). Only the Parser knows when it's at the start of an operand, so it resolves
// these by value instead - see ParserTest.KeywordsAreOnlyRecognizedAtOperandStart.
TEST(LexerTest, KeywordsAreNotSpeciallyTokenized)
{
    Lexer lexer;
    std::string input = "test exit [ ]";
    auto tokens = lexer.TokenizeInput(input);

    ASSERT_EQ(tokens.size(), 4u);

    for (const auto& token : tokens)
        EXPECT_EQ(token.GetSpec().GetTokenType(), TokenSpec::TokenType::SingleCommand);
}

TEST(LexerTest, PlainWordsAreSingleCommandType)
{
    Lexer lexer;
    std::string input = "echo hello-world";
    auto tokens = lexer.TokenizeInput(input);

    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0].GetSpec().GetTokenType(), TokenSpec::TokenType::SingleCommand);
    EXPECT_EQ(tokens[0].GetValue(), "echo");
    EXPECT_EQ(tokens[1].GetSpec().GetTokenType(), TokenSpec::TokenType::SingleCommand);
    EXPECT_EQ(tokens[1].GetValue(), "hello-world");
}

TEST(LexerTest, StopsAtCommentMarker)
{
    Lexer lexer;
    std::string input = "echo hi # ignored words";
    auto tokens = lexer.TokenizeInput(input);

    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0].GetValue(), "echo");
    EXPECT_EQ(tokens[1].GetValue(), "hi");
}

TEST(LexerTest, EmptyInputProducesNoTokens)
{
    Lexer lexer;
    std::string input;
    auto tokens = lexer.TokenizeInput(input);

    EXPECT_TRUE(tokens.empty());
}

}  // namespace
