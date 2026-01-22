#include "AndCommand.h"

AndCommand::AndCommand()
{
    SetTokenType(TokenSpec::TokenType::And);
}

bool AndCommand::Execute()
{
    if (!GetLeft() || !GetRight())
        return false;

    if (GetLeft()->Execute())
        return GetRight()->Execute();

    return false;
}
