#pragma once

#include "Command/Command.h"
#include "Utils/Ref.h"

#include <vector>

class Executor
{
public:
    void Execute(std::vector<Ref<Command>> postfix);
};