#include "Parser.h"
#include "Command/Command.h"
#include "Command/SingleCommand.h"
#include "Command/OrCommand.h"
#include "Command/AndCommand.h"
#include "Command/SemicolonCommand.h"
#include "Command/ExitCommand.h"
#include "Command/TestCommand.h"
#include "Command/ParenthesisCommand.h"
#include "Utils/LoggingUtils.h"

std::vector<Parser::Ref<Command>> Parser::Parse(const std::vector<Token>& tokens)
{
    Reset();

    mTokens = tokens;

    ConstructCommands();

    Utils::Logging::LogMessage("Infix Commands: ");

    for (const auto& i : mInfixCommands)
        Utils::Logging::LogMessage(Utils::Logging::TokenTypeToString(i->GetTokenSpecification().GetTokenType()) + " ");
    Utils::Logging::LogMessage("\n");

    ConstructPostfix();

    Utils::Logging::LogMessage("Postfix Commands: ");

    for (const auto& i : mPostfixCommands)
        Utils::Logging::LogMessage(Utils::Logging::TokenTypeToString(i->GetTokenSpecification().GetTokenType()) + " ");
    Utils::Logging::LogMessage("\n");

    return mPostfixCommands;
}

void Parser::ConstructCommands()
{
    std::vector<Token> args;

    for (const auto& token : mTokens)
    {
        if (token.GetSpecification().GetTokenBaseType() == 
            Token::TokenSpecification::TokenBaseType::Connector)
        {
            if (!args.empty())
            {
                mInfixCommands.emplace_back(CreateCommand(args.front(), args));

                args.clear();
            }

            auto connector = CreateCommand(token);
            connector->AddArgument(token);

            mInfixCommands.emplace_back(connector);
        }
        else
            args.emplace_back(token);
    }

    if (!args.empty())
        mInfixCommands.emplace_back(CreateCommand(args.front(), args));
}

bool Parser::ConstructPostfix()
{
    for (const auto& cmd : mInfixCommands)
    {
        switch (mShuntingState)
        {
        case ShuntingState::ExpectOperand:
            if (ProcessLeftBracket(cmd))
                break;

            if (ProcessRightBracket(cmd))
            {
                LogError(ErrorState::OperandError);

                return false;
            }
            if (ProcessConnectorsPostfix(cmd))
            {
                LogError(ErrorState::OperandError);

                return false;
            }

            mPostfixCommands.emplace_back(cmd);
            mShuntingState = ShuntingState::ExpectConnector;
            break;
        case ShuntingState::ExpectConnector:
            if (ProcessRightBracket(cmd))
                break;

            if (ProcessLeftBracket(cmd))
            {
                LogError(ErrorState::ConnectorError);

                return false;
            }

            if (!ProcessConnectorsPostfix(cmd))
            {
                LogError(ErrorState::OperandError);

                return false;
            }

            mShuntingState = ShuntingState::ExpectOperand;
            break;
        default:
            break;
        }
    }

    if (mShuntingState != ShuntingState::ExpectConnector)
    {
        LogError(ErrorState::ConnectorError);

        return false;
    }

    while (!mConnectors.empty())
    {
        if (mConnectors.top()->GetTokenSpecification().GetTokenBaseType() ==
            Token::TokenSpecification::TokenBaseType::Bracket)
        {
            LogError(ErrorState::BracketError);

            return false;
        }

        mPostfixCommands.emplace_back(mConnectors.top());
        mConnectors.pop();
    }

    return true;
}

bool Parser::ProcessConnectorsPostfix(const Ref<Command>& cmd)
{
    if (cmd->GetTokenSpecification().GetTokenBaseType() ==
        Token::TokenSpecification::TokenBaseType::Connector)
    {
        while (!mConnectors.empty())
        {
            mPostfixCommands.emplace_back(mConnectors.top());
            mConnectors.pop();
        }

        mConnectors.emplace(cmd);

        return true;
    }

    return false;
}

bool Parser::ProcessLeftBracket(const Ref<Command>& cmd)
{
    if (cmd->GetTokenSpecification().GetTokenType() ==
        Token::TokenSpecification::TokenType::LeftParenthesis ||
        cmd->GetTokenSpecification().GetTokenType() ==
        Token::TokenSpecification::TokenType::LeftLegacyTest)
    {
        mConnectors.emplace(cmd);

        return true;
    }

    return false;
}

bool Parser::ProcessRightBracket(const Ref<Command>& cmd)
{
    if (cmd->GetTokenSpecification().GetTokenType() ==
        Token::TokenSpecification::TokenType::RightParenthesis)
    {
        while (!mConnectors.empty())
        {
            if (mConnectors.top()->GetTokenSpecification().GetTokenType() !=
                Token::TokenSpecification::TokenType::LeftParenthesis)
            {
                mConnectors.pop();

                break;
            }

            mPostfixCommands.emplace_back(mConnectors.top());
            mConnectors.pop();
        }

        return true;
    }

    if (cmd->GetTokenSpecification().GetTokenType() ==
        Token::TokenSpecification::TokenType::RightLegacyTest)
    {
        while (!mConnectors.empty())
        {
            if (mConnectors.top()->GetTokenSpecification().GetTokenType() !=
                Token::TokenSpecification::TokenType::LeftLegacyTest)
            {
                mConnectors.pop();

                break;
            }

            mPostfixCommands.emplace_back(mConnectors.top());
            mConnectors.pop();
        }

        return true;
    }

    return false;
}

Parser::Ref<Command> Parser::CreateCommand(const Token& token, 
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
        Utils::Logging::LogMessage("Unexpected token: expected Operand.\n",
                                   Utils::Logging::LogType::Error);
        break;
    case ErrorState::ConnectorError:
        Utils::Logging::LogMessage("Unexpected token: expected Connector.\n",
                                   Utils::Logging::LogType::Error);
        break;
    case ErrorState::BracketError:
        Utils::Logging::LogMessage("Unexpected token: expected Bracket.\n",
                                   Utils::Logging::LogType::Error);
        break;
    default:
        break;
    }
}

void Parser::Reset()
{
    mInfixCommands.clear();
    mPostfixCommands.clear();
    mConnectors = {};
    mShuntingState = ShuntingState::ExpectOperand;
}