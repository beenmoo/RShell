#pragma once

#include "Token/Token.h"

#include <vector>
#include <string>

class Lexer
{
public:
    std::vector<Token> TokenizeInput(std::string& input);

private:
    std::string mInput;
};