#include "Lexer.h"

#include <sstream>
#include <stack>

std::vector<Token> Lexer::TokenizeInput(std::string& input)
{
    Reset();

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
            token.SetTokenType(Token::TokenSpecification::TokenType::Or);
        else if (tokenValue == "&&")
            token.SetTokenType(Token::TokenSpecification::TokenType::And);
        else if (tokenValue == ";")
            token.SetTokenType(Token::TokenSpecification::TokenType::Semicolon);
        else if (tokenValue == "exit")
            token.SetTokenType(Token::TokenSpecification::TokenType::Exit);
        else if (tokenValue == "test")
            token.SetTokenType(Token::TokenSpecification::TokenType::Test);
        else if (tokenValue == "[")
            token.SetTokenType(Token::TokenSpecification::TokenType::LeftLegacyTest);
        else if (tokenValue == "]")
            token.SetTokenType(Token::TokenSpecification::TokenType::RightLegacyTest);
        else if (tokenValue == "(")
            token.SetTokenType(Token::TokenSpecification::TokenType::LeftParenthesis);
        else if (tokenValue == ")")
            token.SetTokenType(Token::TokenSpecification::TokenType::RightParenthesis);
        else
            token.SetTokenType(Token::TokenSpecification::TokenType::SingleCommand);

        token.SetValue(tokenValue);

        tokens.emplace_back(token);
    }

    return tokens;
}

bool Lexer::AdvanceCursor()
{
    if (mCursor + 1 >= mInput.size())
        return false;

    ++mCursor;

    return false;
}

char Lexer::GetToken() const
{
    return mInput[mCursor];
}

void Lexer::Reset()
{
    mCursor = 0;
}
