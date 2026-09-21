#include "Parser.h"

#include "Command/AndCommand.h"
#include "Command/ExitCommand.h"
#include "Command/OrCommand.h"
#include "Command/SemicolonCommand.h"
#include "Command/SingleCommand.h"
#include "Command/TestCommand.h"
#include "Utils/Logging.h"

std::shared_ptr<Command> Parser::Parse(const std::vector<Token>& tokens)
{
    mTokens = tokens;
    mPos = 0;

    if (mTokens.empty())
        return nullptr;

    auto root = ParseExpression();

    if (root != nullptr && mPos != mTokens.size())
    {
        LogError(ErrorState::ConnectorError);
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
            LogError(ErrorState::BracketError);
            return nullptr;
        }

        ++mPos;

        return expr;
    }
    case TokenSpec::TokenType::LeftLegacyTest:
        ++mPos;
        return ParseTest(TokenSpec::TokenType::RightLegacyTest);
    case TokenSpec::TokenType::Test:
        ++mPos;
        return ParseTest(TokenSpec::TokenType::None);
    case TokenSpec::TokenType::Exit:
        ++mPos;
        return std::make_shared<ExitCommand>();
    case TokenSpec::TokenType::SingleCommand:
        return ParseSingleCommand();
    default:
        LogError(ErrorState::OperandError);
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

std::shared_ptr<Command> Parser::ParseTest(TokenSpec::TokenType closingType)
{
    if (PeekType() != TokenSpec::TokenType::SingleCommand)
    {
        LogError(ErrorState::OperandError);
        return nullptr;
    }

    const std::string flag = mTokens[mPos].GetValue();
    ++mPos;

    if (PeekType() != TokenSpec::TokenType::SingleCommand)
    {
        LogError(ErrorState::OperandError);
        return nullptr;
    }

    const std::string path = mTokens[mPos].GetValue();
    ++mPos;

    if (closingType != TokenSpec::TokenType::None)
    {
        if (PeekType() != closingType)
        {
            LogError(ErrorState::BracketError);
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

void Parser::LogError(ErrorState state) const
{
    switch (state)
    {
    case ErrorState::OperandError:
        LOG_ERROR("Unexpected token: expected Operand.\n");
        break;
    case ErrorState::ConnectorError:
        LOG_ERROR("Unexpected token: expected Connector.\n");
        break;
    case ErrorState::BracketError:
        LOG_ERROR("Unexpected token: expected Bracket.\n");
        break;
    }
}
