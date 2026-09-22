#pragma once

#include "Token/Token.h"

#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

class Command;

class Parser
{
public:
    std::shared_ptr<Command> Parse(const std::vector<Token>& tokens);

private:
    std::shared_ptr<Command> ParseExpression(int minPrecedence = 1);
    std::shared_ptr<Command> ParseOperand();
    std::shared_ptr<Command> ParseSingleCommand();
    std::shared_ptr<Command> ParseTest(bool requireClosingBracket);

    static int GetPrecedence(TokenSpec::TokenType type);
    static std::shared_ptr<Command> MakeComposite(TokenSpec::TokenType opType,
                                                  std::shared_ptr<Command> left,
                                                  std::shared_ptr<Command> right);

    TokenSpec::TokenType PeekType() const;

    void LogSyntaxError(std::string_view expected) const;

    std::vector<Token> mTokens;
    std::size_t mPos = 0;
};
