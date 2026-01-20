#include "Parser.h"
#include "Command/Command.h"
#include "Command/ConnectorCommand.h"
#include "Command/SingleCommand.h"
#include "Command/OrCommand.h"
#include "Command/AndCommand.h"
#include "Command/SemicolonCommand.h"
#include "Command/ExitCommand.h"
#include "Command/TestCommand.h"
#include "Command/ParenthesisCommand.h"
#include "Utils/LoggingUtils.h"

Ref<Command> Parser::Parse(const std::vector<Token>& tokens)
{
    Reset();

    mTokens = tokens;
    mCursor = 0;

    auto root = ParseExpression();
    if (!root)
        return nullptr;

    if (HasMoreTokens())
    {
        LogError(ErrorState::ConnectorError);

        return nullptr;
    }

    return root;
}

Ref<Command> Parser::ParseExpression()
{
    return ParseSequence();
}

Ref<Command> Parser::ParseSequence()
{
    auto left = ParseOr();
    if (!left)
        return nullptr;

    while (Match(Token::TokenSpecification::TokenType::Semicolon))
    {
        auto right = ParseOr();
        if (!right)
            return nullptr;

        auto connector = CreateCommand(mTokens[mCursor - 1],
                                       { mTokens[mCursor - 1] });
        auto connectorCmd = std::dynamic_pointer_cast<ConnectorCommand>(connector);
        connectorCmd->SetLeft(left);
        connectorCmd->SetRight(right);
        left = connector;
    }

    return left;
}

Ref<Command> Parser::ParseOr()
{
    auto left = ParseAnd();
    if (!left)
        return nullptr;

    while (Match(Token::TokenSpecification::TokenType::Or))
    {
        auto right = ParseAnd();
        if (!right)
            return nullptr;

        auto connector = CreateCommand(mTokens[mCursor - 1],
                                       { mTokens[mCursor - 1] });
        auto connectorCmd = std::dynamic_pointer_cast<ConnectorCommand>(connector);
        connectorCmd->SetLeft(left);
        connectorCmd->SetRight(right);
        left = connector;
    }

    return left;
}

Ref<Command> Parser::ParseAnd()
{
    auto left = ParsePrimary();
    if (!left)
        return nullptr;

    while (Match(Token::TokenSpecification::TokenType::And))
    {
        auto right = ParsePrimary();
        if (!right)
            return nullptr;

        auto connector = CreateCommand(mTokens[mCursor - 1],
                                       { mTokens[mCursor - 1] });
        auto connectorCmd = std::dynamic_pointer_cast<ConnectorCommand>(connector);
        connectorCmd->SetLeft(left);
        connectorCmd->SetRight(right);
        left = connector;
    }

    return left;
}

Ref<Command> Parser::ParsePrimary()
{
    if (!HasMoreTokens())
    {
        LogError(ErrorState::OperandError);

        return nullptr;
    }

    const auto type = PeekToken().GetSpecification().GetTokenType();

    if (type == Token::TokenSpecification::TokenType::LeftParenthesis)
    {
        Match(Token::TokenSpecification::TokenType::LeftParenthesis);

        auto expr = ParseExpression();
        if (!expr)
            return nullptr;

        if (!Match(Token::TokenSpecification::TokenType::RightParenthesis))
        {
            LogError(ErrorState::BracketError);

            return nullptr;
        }

        return expr;
    }

    if (type == Token::TokenSpecification::TokenType::LeftLegacyTest)
    {
        Match(Token::TokenSpecification::TokenType::LeftLegacyTest);

        std::vector<Token> legacyArgs;
        legacyArgs.emplace_back(mTokens[mCursor - 1]);

        while (HasMoreTokens() &&
               PeekToken().GetSpecification().GetTokenType() !=
                   Token::TokenSpecification::TokenType::RightLegacyTest)
        {
            legacyArgs.emplace_back(PeekToken());
            ++mCursor;
        }

        if (!Match(Token::TokenSpecification::TokenType::RightLegacyTest))
        {
            LogError(ErrorState::BracketError);

            return nullptr;
        }

        legacyArgs.emplace_back(mTokens[mCursor - 1]);

        auto legacyCmd = CreateRef<TestCommand>();
        legacyCmd->SetArguments(legacyArgs);
        return legacyCmd;
    }

    return ParseCommand();
}

Ref<Command> Parser::ParseCommand()
{
    if (!HasMoreTokens())
    {
        LogError(ErrorState::OperandError);

        return nullptr;
    }

    const auto type = PeekToken().GetSpecification().GetTokenType();

    if (!IsOperandStart(type))
    {
        if (IsDelimiterToken(type))
            LogError(ErrorState::OperandError);
        else
            LogError(ErrorState::ConnectorError);

        return nullptr;
    }

    std::vector<Token> args;

    while (HasMoreTokens() && !IsDelimiterToken(PeekToken().GetSpecification().GetTokenType()))
    {
        const auto nextType = PeekToken().GetSpecification().GetTokenType();

        if (nextType == Token::TokenSpecification::TokenType::LeftParenthesis ||
            nextType == Token::TokenSpecification::TokenType::LeftLegacyTest)
        {
            LogError(ErrorState::ConnectorError);

            return nullptr;
        }

        args.emplace_back(PeekToken());
        ++mCursor;
    }

    if (args.empty())
    {
        LogError(ErrorState::OperandError);

        return nullptr;
    }

    return CreateCommand(args.front(), args);
}

bool Parser::HasMoreTokens() const
{
    return mCursor < mTokens.size();
}

const Token& Parser::PeekToken() const
{
    return mTokens[mCursor];
}

bool Parser::Match(Token::TokenSpecification::TokenType type)
{
    if (!HasMoreTokens())
        return false;

    if (PeekToken().GetSpecification().GetTokenType() != type)
        return false;

    ++mCursor;

    return true;
}

bool Parser::IsConnectorToken(Token::TokenSpecification::TokenType type) const
{
    return type == Token::TokenSpecification::TokenType::And ||
           type == Token::TokenSpecification::TokenType::Or ||
           type == Token::TokenSpecification::TokenType::Semicolon;
}

bool Parser::IsDelimiterToken(Token::TokenSpecification::TokenType type) const
{
    return IsConnectorToken(type) ||
           type == Token::TokenSpecification::TokenType::RightParenthesis ||
           type == Token::TokenSpecification::TokenType::RightLegacyTest;
}

bool Parser::IsOperandStart(Token::TokenSpecification::TokenType type) const
{
    return type == Token::TokenSpecification::TokenType::SingleCommand ||
           type == Token::TokenSpecification::TokenType::Exit ||
           type == Token::TokenSpecification::TokenType::Test;
}

Ref<Command> Parser::CreateCommand(const Token& token, 
                                           const std::vector<Token>& args)
{
    Ref<Command> cmd = nullptr;

    switch (token.GetSpecification().GetTokenType())
    {
    case Token::TokenSpecification::TokenType::And:
        cmd = CreateRef<AndCommand>();
        break;
    case Token::TokenSpecification::TokenType::Or:
        cmd = CreateRef<OrCommand>();
        break;
    case Token::TokenSpecification::TokenType::Semicolon:
        cmd = CreateRef<SemicolonCommand>();
        break;
    case Token::TokenSpecification::TokenType::SingleCommand:
        cmd = CreateRef<SingleCommand>();
        break;
    case Token::TokenSpecification::TokenType::Exit:
        cmd = CreateRef<ExitCommand>();
        break;
    case Token::TokenSpecification::TokenType::Test:
        cmd = CreateRef<TestCommand>();
        break;
    case Token::TokenSpecification::TokenType::LeftLegacyTest:
        cmd = CreateRef<LeftLegacyTestCommand>();
        break;
    case Token::TokenSpecification::TokenType::RightLegacyTest:
        cmd = CreateRef<RightLegacyTestCommand>();
        break;
    case Token::TokenSpecification::TokenType::LeftParenthesis:
        cmd = CreateRef<LeftParenthesisCommand>();
        break;
    case Token::TokenSpecification::TokenType::RightParenthesis:
        cmd = CreateRef<RightParenthesisCommand>();
        break;
    default:
        break;
    }

    cmd->SetArguments(args);

    return cmd;
}

void Parser::LogError(ErrorState state) const
{
    switch (state)
    {
    case ErrorState::OperandError:
        LOG_MESSAGE("Unexpected token: expected Operand.\n",
                    Logging::LogType::Error);
        break;
    case ErrorState::ConnectorError:
        LOG_MESSAGE("Unexpected token: expected Connector.\n",
                    Logging::LogType::Error);
        break;
    case ErrorState::BracketError:
        LOG_MESSAGE("Unexpected token: expected Bracket.\n",
                    Logging::LogType::Error);
        break;
    default:
        break;
    }
}

void Parser::Reset()
{
    mTokens.clear();
    mCursor = 0;
}