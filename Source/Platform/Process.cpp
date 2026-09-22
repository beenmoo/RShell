#include "Process.h"

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace Platform
{

#ifdef _WIN32

namespace
{
std::string QuoteArgument(const std::string& arg)
{
    if (!arg.empty() && arg.find_first_of(" \t\"") == std::string::npos)
        return arg;

    std::string quoted = "\"";

    for (auto it = arg.begin();; ++it)
    {
        size_t backslashCount = 0;

        while (it != arg.end() && *it == '\\')
        {
            ++it;
            ++backslashCount;
        }

        if (it == arg.end())
        {
            quoted.append(backslashCount * 2, '\\');
            break;
        }

        if (*it == '"')
        {
            quoted.append((backslashCount * 2) + 1, '\\');
            quoted.push_back('"');
        }
        else
        {
            quoted.append(backslashCount, '\\');
            quoted.push_back(*it);
        }
    }

    quoted.push_back('"');
    return quoted;
}

std::string BuildCommandLine(const std::vector<std::string>& args)
{
    std::string commandLine;

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i > 0)
            commandLine.push_back(' ');

        commandLine += QuoteArgument(args[i]);
    }

    return commandLine;
}

std::string FormatWindowsError(DWORD error)
{
    LPSTR buffer = nullptr;

    // FORMAT_MESSAGE_ALLOCATE_BUFFER's contract requires the output pointer as an LPSTR* passed
    // through an LPSTR-typed parameter - there's no way to satisfy this WinAPI idiom without a
    // reinterpret_cast.
    const DWORD length =
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                           FORMAT_MESSAGE_IGNORE_INSERTS,
                       nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       reinterpret_cast<LPSTR>(&buffer), 0,  // NOLINT(*-reinterpret-cast)
                       nullptr);

    std::string message = (length > 0 && buffer) ? std::string(buffer, length) : "Unknown error";

    if (buffer)
        LocalFree(buffer);

    while (!message.empty() && (message.back() == '\n' || message.back() == '\r'))
        message.pop_back();

    return message;
}
}  // namespace

bool RunProcess(const std::vector<std::string>& args)
{
    if (args.empty())
        return false;

    std::string commandLine = BuildCommandLine(args);

    STARTUPINFOA startupInfo{};
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInfo{};

    const BOOL created = CreateProcessA(nullptr, commandLine.data(), nullptr, nullptr, TRUE, 0,
                                        nullptr, nullptr, &startupInfo, &processInfo);

    if (created == 0)
    {
        const DWORD error = GetLastError();

        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND)
            std::cout << "Shell: " << args[0] << ": command not found\n";
        else
            std::cout << "Shell: " << args[0] << ": " << FormatWindowsError(error) << "\n";

        return false;
    }

    WaitForSingleObject(processInfo.hProcess, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeProcess(processInfo.hProcess, &exitCode);

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    return exitCode == 0;
}

#else

bool RunProcess(const std::vector<std::string>& args)
{
    if (args.empty())
        return false;

    std::vector<char*> argv;
    argv.reserve(args.size() + 1);

    for (const auto& arg : args)
        argv.emplace_back(const_cast<char*>(arg.c_str()));

    argv.emplace_back(nullptr);

    pid_t pid = fork();

    if (pid > 0)
    {
        int status = 0;

        if (waitpid(pid, &status, 0) == -1)
        {
            perror("wait");

            return false;
        }

        return WEXITSTATUS(status) == 0;
    }
    else if (pid == 0)
    {
        if (execvp(argv[0], argv.data()) == -1)
        {
            if (errno == ENOENT)
                std::cout << "Shell: " << args[0] << ": command not found\n";
            else
                std::cout << "Shell: " << args[0] << ": " << std::strerror(errno) << "\n";

            _exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("fork");

        return false;
    }

    return true;
}

#endif

}  // namespace Platform
