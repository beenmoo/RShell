#include "AndCommand.h"

AndCommand::AndCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::And);
}

bool AndCommand::Execute()
{
    if (GetLeftCommand()->Execute())
        return GetRightCommand()->Execute();

    return false;
}
