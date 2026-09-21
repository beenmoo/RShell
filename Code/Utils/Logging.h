#pragma once

#include <cstdint>
#include <string>
#include <iostream>

class Logger
{
public:
    enum class LogType : std::uint8_t
    {
        None,
        Trace,
        Info,
        Error
    };

    Logger() = default;

    void LogMessage(const std::string& msg, LogType type = LogType::None) const
    {
        std::string outputMsg;

        switch (type)
        {
        case LogType::Trace:
            outputMsg = "Trace: " + msg;
            break;
        case LogType::Info:
            outputMsg = "Info: " + msg;
            break;
        case LogType::Error:
            outputMsg = "Error: " + msg;
            break;
        default:
            outputMsg = msg;
            break;
        }

        std::cout << outputMsg;
    }

    static void SetInstance(Logger* instance)
    {
        sInstance = instance;
    }

    static Logger& Get()
    {
        if (sInstance != nullptr)
            return *sInstance;

        static Logger sFallback = {};
        return sFallback;
    }

private:
    static inline Logger* sInstance = nullptr;
};

#define LOG_MESSAGE(msg) ::Logger::Get().LogMessage((msg), ::Logger::LogType::None)
#define LOG_TRACE(msg) ::Logger::Get().LogMessage((msg), ::Logger::LogType::Trace)
#define LOG_INFO(msg) ::Logger::Get().LogMessage((msg), ::Logger::LogType::Info)
#define LOG_ERROR(msg) ::Logger::Get().LogMessage((msg), ::Logger::LogType::Error)
