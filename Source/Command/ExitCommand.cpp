#include "ExitCommand.h"

#include <cstdlib>

bool ExitCommand::Execute()
{
    exit(EXIT_SUCCESS);
}
