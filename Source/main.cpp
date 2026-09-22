#include "Core/Shell.h"

#include <cstdlib>
#include <exception>
#include <iostream>

int main()
{
    try
    {
        Shell shell;

        shell.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
