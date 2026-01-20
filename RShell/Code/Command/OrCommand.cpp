#include "OrCommand.h"

OrCommand::OrCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::Or);
}

bool OrCommand::Execute()
{
    if (!GetLeft()->Execute())
        return GetRight()->Execute();

    return false;
}
