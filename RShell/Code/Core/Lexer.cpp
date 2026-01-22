#include "Lexer.h"

#include <sstream>
#include <stack>

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

        if (tokenValue == "||")
            token.SetTokenType(TokenSpec::TokenType::Or);
        else if (tokenValue == "&&")
            token.SetTokenType(TokenSpec::TokenType::And);
        else if (tokenValue == ";")
            token.SetTokenType(TokenSpec::TokenType::Semicolon);
        else if (tokenValue == "exit")
            token.SetTokenType(TokenSpec::TokenType::Exit);
        else if (tokenValue == "test")
            token.SetTokenType(TokenSpec::TokenType::Test);
        else if (tokenValue == "[")
            token.SetTokenType(TokenSpec::TokenType::LeftLegacyTest);
        else if (tokenValue == "]")
            token.SetTokenType(TokenSpec::TokenType::RightLegacyTest);
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