#pragma once

#include "Token/Token.h"

#include <vector>
#include <string>

class Lexer
{
private:
    enum class LexerState
    {
        State
    };

public:
    std::vector<Token> TokenizeInput(std::string& input);

private:
    bool AdvanceCursor();
    char GetToken() const;

    void Reset();

private:
    std::string mInput;

    size_t mCursor = 0;
};