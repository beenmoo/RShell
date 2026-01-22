#pragma once

#include "Token/Token.h"
#include "Utils/Ref.h"

#include <vector>

class Command;

class CommandFactory
{
public:
    static Ref<Command> Create(const Token& token,
                               const std::vector<Token>& args = {});
};
