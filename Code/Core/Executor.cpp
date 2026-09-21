#include "Executor.h"

void Executor::Execute(const std::shared_ptr<Command>& root)
{
    if (root != nullptr)
        root->Execute();
}
