#include "CommandFactory.h"

#include "Command.h"
#include "AndCommand.h"
#include "OrCommand.h"
#include "SemicolonCommand.h"
#include "SingleCommand.h"
#include "ExitCommand.h"
#include "TestCommand.h"
#include "ParenthesisCommand.h"

Ref<Command> CommandFactory::Create(const Token& token,
                                    const std::vector<Token>& args)
{
    Ref<Command> cmd = nullptr;

    switch (token.GetSpec().GetTokenType())
    {
    case TokenSpec::TokenType::And:
        cmd = CreateRef<AndCommand>();
        break;
    case TokenSpec::TokenType::Or:
        cmd = CreateRef<OrCommand>();
        break;
    case TokenSpec::TokenType::Semicolon:
        cmd = CreateRef<SemicolonCommand>();
        break;
    case TokenSpec::TokenType::SingleCommand:
        cmd = CreateRef<SingleCommand>();
        break;
    case TokenSpec::TokenType::Exit:
        cmd = CreateRef<ExitCommand>();
        break;
    case TokenSpec::TokenType::Test:
        cmd = CreateRef<TestCommand>();
        break;
    case TokenSpec::TokenType::LeftLegacyTest:
        cmd = CreateRef<LeftLegacyTestCommand>();
        break;
    case TokenSpec::TokenType::RightLegacyTest:
        cmd = CreateRef<RightLegacyTestCommand>();
        break;
    case TokenSpec::TokenType::LeftParenthesis:
        cmd = CreateRef<LeftParenthesisCommand>();
        break;
    case TokenSpec::TokenType::RightParenthesis:
        cmd = CreateRef<RightParenthesisCommand>();
        break;
    default:
        break;
    }

    if (!cmd)
        return nullptr;

    cmd->SetArguments(args);

    return cmd;
}
