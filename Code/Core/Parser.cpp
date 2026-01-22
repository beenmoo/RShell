#include "Parser.h"
#include "Command/Command.h"
#include "Command/CommandFactory.h"
#include "Utils/Logging.h"

std::vector<Ref<Command>> Parser::Parse(const std::vector<Token>& tokens)
{
    Reset();
    mTokens = tokens;
    ConstructCommands();
    ConstructPostfix();

    return mPostfixCommands;
}

void Parser::ConstructCommands()
{
    std::vector<Token> args;

    for (const auto& token : mTokens)
    {
        if (token.GetSpec().GetTokenBaseType() == 
            TokenSpec::TokenBaseType::Connector)
        {
            if (!args.empty())
            {
                mInfixCommands.emplace_back(CommandFactory::Create(args.front(), args));
                args.clear();
            }

            auto connector = CommandFactory::Create(token);
            connector->AddArgument(token);
            mInfixCommands.emplace_back(connector);
        }
        else
            args.emplace_back(token);
    }

    if (!args.empty())
        mInfixCommands.emplace_back(CommandFactory::Create(args.front(), args));
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
        if (mConnectors.top()->GetSpec().GetTokenBaseType() ==
            TokenSpec::TokenBaseType::Bracket)
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
    if (cmd->GetSpec().GetTokenBaseType() ==
        TokenSpec::TokenBaseType::Connector)
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
    if (cmd->GetSpec().GetTokenType() ==
        TokenSpec::TokenType::LeftParenthesis ||
        cmd->GetSpec().GetTokenType() ==
        TokenSpec::TokenType::LeftLegacyTest)
    {
        mConnectors.emplace(cmd);
        return true;
    }

    return false;
}

bool Parser::ProcessRightBracket(const Ref<Command>& cmd)
{
    if (cmd->GetSpec().GetTokenType() ==
        TokenSpec::TokenType::RightParenthesis)
    {
        while (!mConnectors.empty())
        {
            if (mConnectors.top()->GetSpec().GetTokenType() !=
                TokenSpec::TokenType::LeftParenthesis)
            {
                mConnectors.pop();
                break;
            }

            mPostfixCommands.emplace_back(mConnectors.top());
            mConnectors.pop();
        }

        return true;
    }

    if (cmd->GetSpec().GetTokenType() ==
        TokenSpec::TokenType::RightLegacyTest)
    {
        while (!mConnectors.empty())
        {
            if (mConnectors.top()->GetSpec().GetTokenType() !=
                TokenSpec::TokenType::LeftLegacyTest)
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