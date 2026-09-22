#include "Core/Shell.h"

#include <cstdlib>
#include <exception>
#include <iostream>

int main()
{
    try
    {
        Shell shell;

        return shell.Run() ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";

        return EXIT_FAILURE;
    }
}
