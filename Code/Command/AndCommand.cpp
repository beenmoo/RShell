#include "AndCommand.h"

bool AndCommand::Execute()
{
    return Left()->Execute() && Right()->Execute();
}
