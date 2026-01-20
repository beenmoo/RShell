#include "AndCommand.h"

AndCommand::AndCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::And);
}

bool AndCommand::Execute()
{
    if (GetLeft()->Execute())
        return GetRight()->Execute();

    return false;
}
