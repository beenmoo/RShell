#include "ExitCommand.h"

ExitCommand::ExitCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::Exit);
}

bool ExitCommand::Execute()
{
    exit(EXIT_SUCCESS);

    return false;
}
