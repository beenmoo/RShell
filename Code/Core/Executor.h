#pragma once

#include "Command/Command.h"

#include <memory>

class Executor
{
public:
    void Execute(const std::shared_ptr<Command>& root);
};
