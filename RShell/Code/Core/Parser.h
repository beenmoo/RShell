#pragma once

#include "Token/Token.h"
#include "Utils/Ref.h"

#include <string>
#include <vector>
#include <memory>
#include <stack>

class Command;

class Parser
{
private:
    enum class ErrorState
    {
        None,
        OperandError,
        ConnectorError,
        BracketError
    };

public:
    Ref<Command> Parse(const std::vector<Token>& tokens);

private:
    Ref<Command> ParseExpression();
    Ref<Command> ParseSequence();
    Ref<Command> ParseOr();
    Ref<Command> ParseAnd();
    Ref<Command> ParsePrimary();
    Ref<Command> ParseCommand();

    bool HasMoreTokens() const;
    const Token& PeekToken() const;
    bool Match(Token::TokenSpecification::TokenType type);

    bool IsConnectorToken(Token::TokenSpecification::TokenType type) const;
    bool IsDelimiterToken(Token::TokenSpecification::TokenType type) const;
    bool IsOperandStart(Token::TokenSpecification::TokenType type) const;

    Ref<Command> CreateCommand(const Token& token,
                               const std::vector<Token>& args = {});

    void LogError(ErrorState state) const;

    void Reset();

private:
    std::vector<Token> mTokens;
    size_t mCursor = 0;
};