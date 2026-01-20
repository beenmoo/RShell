#pragma once

#include "Utils/Ref.h"

class Command;

class Executor
{
public:
    void Execute(const Ref<Command>& root);
};