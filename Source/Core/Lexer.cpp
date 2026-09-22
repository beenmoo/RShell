#include "Lexer.h"

#include <sstream>

std::vector<Token> Lexer::TokenizeInput(std::string& input)
{
    mInput = input;

    std::istringstream iss(input);
    std::string tokenValue;
    std::vector<Token> tokens;

    Token token;

    while (iss >> tokenValue)
    {
        if (tokenValue == "#")
            break;

        // "test"/"exit"/"["/"]" are deliberately NOT recognized here: unlike these five, they're
        // ordinary words that only mean something special as the first word of a command (e.g.
        // "echo test" must not treat "test" as the test builtin). Only the Parser knows when it's
        // at the start of an operand, so it resolves those by value in ParseOperand instead.
        if (tokenValue == "||")
            token.SetTokenType(TokenSpec::TokenType::Or);
        else if (tokenValue == "&&")
            token.SetTokenType(TokenSpec::TokenType::And);
        else if (tokenValue == ";")
            token.SetTokenType(TokenSpec::TokenType::Semicolon);
        else if (tokenValue == "(")
            token.SetTokenType(TokenSpec::TokenType::LeftParenthesis);
        else if (tokenValue == ")")
            token.SetTokenType(TokenSpec::TokenType::RightParenthesis);
        else
            token.SetTokenType(TokenSpec::TokenType::SingleCommand);

        token.SetValue(tokenValue);

        tokens.emplace_back(token);
    }

    return tokens;
}