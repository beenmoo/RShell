#include "OrCommand.h"

OrCommand::OrCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::Or);
}

bool OrCommand::Execute()
{
    if (!GetLeftCommand()->Execute())
        return GetRightCommand()->Execute();

    return false;
}
