#include "ExitCommand.h"

ExitCommand::ExitCommand()
{
    SetTokenType(TokenSpec::TokenType::Exit);
}

bool ExitCommand::Execute()
{
    exit(EXIT_SUCCESS);

    return false;
}
