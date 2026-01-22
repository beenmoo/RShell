#pragma once

#include "Token/Token.h"

#include <string>
#include <iostream>

class Logger
{
public:
    enum class LogType
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
        if (sInstance)
            return *sInstance;

        static Logger fallback = {};
        return fallback;
    }

    static std::string TokenBaseTypeToString(TokenSpec::TokenBaseType type)
    {
        switch (type)
        {
        case TokenSpec::TokenBaseType::Operand:
            return "Operand";
        case TokenSpec::TokenBaseType::Connector:
            return "Connector";
        default:
            break;
        }

        return "None";
    }

    static std::string TokenTypeToString(TokenSpec::TokenType type)
    {
        switch (type)
        {
        case TokenSpec::TokenType::SingleCommand:
            return "SingleCommand";
        case TokenSpec::TokenType::Exit:
            return "Exit";
        case TokenSpec::TokenType::Or:
            return "Or";
        case TokenSpec::TokenType::And:
            return "And";
        case TokenSpec::TokenType::Semicolon:
            return "Semicolon";
        default:
            break;
        }

        return "None";
    }

private:
    static inline Logger* sInstance = nullptr;
};

#define LOG_MESSAGE(msg) ::Logger::Get().LogMessage((msg), ::Logger::LogType::None)
#define LOG_TRACE(msg) ::Logger::Get().LogMessage((msg), ::Logger::LogType::Trace)
#define LOG_INFO(msg) ::Logger::Get().LogMessage((msg), ::Logger::LogType::Info)
#define LOG_ERROR(msg) ::Logger::Get().LogMessage((msg), ::Logger::LogType::Error)