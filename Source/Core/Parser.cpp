#include "Parser.h"

#include "Command/AndCommand.h"
#include "Command/ExitCommand.h"
#include "Command/OrCommand.h"
#include "Command/SemicolonCommand.h"
#include "Command/SingleCommand.h"
#include "Command/TestCommand.h"

#include <iostream>

std::shared_ptr<Command> Parser::Parse(const std::vector<Token>& tokens)
{
    mTokens = tokens;
    mPos = 0;

    if (mTokens.empty())
        return nullptr;

    auto root = ParseExpression();

    if (root != nullptr && mPos != mTokens.size())
    {
        LogSyntaxError("end of input");
        return nullptr;
    }

    return root;
}

std::shared_ptr<Command> Parser::ParseExpression(int minPrecedence)
{
    auto left = ParseOperand();

    if (left == nullptr)
        return nullptr;

    while (true)
    {
        const TokenSpec::TokenType opType = PeekType();
        const int opPrecedence = GetPrecedence(opType);

        if (opPrecedence == 0 || opPrecedence < minPrecedence)
            break;

        ++mPos;

        auto right = ParseExpression(opPrecedence + 1);

        if (right == nullptr)
            return nullptr;

        left = MakeComposite(opType, std::move(left), std::move(right));
    }

    return left;
}

std::shared_ptr<Command> Parser::ParseOperand()
{
    switch (PeekType())
    {
    case TokenSpec::TokenType::LeftParenthesis: {
        ++mPos;

        auto expr = ParseExpression();

        if (expr == nullptr)
            return nullptr;

        if (PeekType() != TokenSpec::TokenType::RightParenthesis)
        {
            LogSyntaxError("')'");
            return nullptr;
        }

        ++mPos;

        return expr;
    }
    case TokenSpec::TokenType::SingleCommand: {
        const std::string& value = mTokens[mPos].GetValue();

        if (value == "[")
        {
            ++mPos;
            return ParseTest(true);
        }
        if (value == "test")
        {
            ++mPos;
            return ParseTest(false);
        }
        if (value == "exit")
        {
            ++mPos;
            return std::make_shared<ExitCommand>();
        }

        return ParseSingleCommand();
    }
    default:
        LogSyntaxError("an operand");
        return nullptr;
    }
}

std::shared_ptr<Command> Parser::ParseSingleCommand()
{
    std::vector<std::string> args;

    while (PeekType() == TokenSpec::TokenType::SingleCommand)
    {
        args.emplace_back(mTokens[mPos].GetValue());
        ++mPos;
    }

    return std::make_shared<SingleCommand>(std::move(args));
}

std::shared_ptr<Command> Parser::ParseTest(bool requireClosingBracket)
{
    if (PeekType() != TokenSpec::TokenType::SingleCommand)
    {
        LogSyntaxError("a test flag");
        return nullptr;
    }

    const std::string flag = mTokens[mPos].GetValue();
    ++mPos;

    if (PeekType() != TokenSpec::TokenType::SingleCommand)
    {
        LogSyntaxError("a path");
        return nullptr;
    }

    const std::string path = mTokens[mPos].GetValue();
    ++mPos;

    if (requireClosingBracket)
    {
        if (PeekType() != TokenSpec::TokenType::SingleCommand || mTokens[mPos].GetValue() != "]")
        {
            LogSyntaxError("']'");
            return nullptr;
        }

        ++mPos;
    }

    return std::make_shared<TestCommand>(flag, path);
}

int Parser::GetPrecedence(TokenSpec::TokenType type)
{
    switch (type)
    {
    case TokenSpec::TokenType::Semicolon:
        return 1;
    case TokenSpec::TokenType::And:
    case TokenSpec::TokenType::Or:
        return 2;
    default:
        return 0;
    }
}

std::shared_ptr<Command> Parser::MakeComposite(TokenSpec::TokenType opType,
                                               std::shared_ptr<Command> left,
                                               std::shared_ptr<Command> right)
{
    switch (opType)
    {
    case TokenSpec::TokenType::And:
        return std::make_shared<AndCommand>(std::move(left), std::move(right));
    case TokenSpec::TokenType::Or:
        return std::make_shared<OrCommand>(std::move(left), std::move(right));
    case TokenSpec::TokenType::Semicolon:
        return std::make_shared<SemicolonCommand>(std::move(left), std::move(right));
    default:
        return nullptr;
    }
}

TokenSpec::TokenType Parser::PeekType() const
{
    if (mPos >= mTokens.size())
        return TokenSpec::TokenType::None;

    return mTokens[mPos].GetSpec().GetTokenType();
}

void Parser::LogSyntaxError(std::string_view expected) const
{
    std::cout << "Syntax error: expected " << expected << ", but found ";

    if (mPos < mTokens.size())
        std::cout << "'" << mTokens[mPos].GetValue() << "'";
    else
        std::cout << "end of input";

    std::cout << ".\n";
}
