#include "OrCommand.h"

OrCommand::OrCommand()
{
    SetTokenType(TokenSpec::TokenType::Or);
}

bool OrCommand::Execute()
{
    if (!GetLeft() || !GetRight())
        return false;

    if (!GetLeft()->Execute())
        return GetRight()->Execute();

    return true;
}
