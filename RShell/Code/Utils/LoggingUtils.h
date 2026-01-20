#pragma once

#include "Token/Token.h"

#include <string>
#include <iostream>

namespace Logging
{
    enum class LogType
    {
        None,
        Trace,
        Info,
        Error
    };

    static inline void LogMessage(const std::string& msg, LogType type = LogType::None)
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

    static inline std::string TokenBaseTypeToString(Token::TokenSpecification::TokenBaseType type)
    {
        switch (type)
        {
        case Token::TokenSpecification::TokenBaseType::Operand:
            return "Operand";
        case Token::TokenSpecification::TokenBaseType::Connector:
            return "Connector";
        default:
            break;
        }

        return "None";
    }

    static inline std::string TokenTypeToString(Token::TokenSpecification::TokenType type)
    {
        switch (type)
        {
        case Token::TokenSpecification::TokenType::SingleCommand:
            return "SingleCommand";
        case Token::TokenSpecification::TokenType::Exit:
            return "Exit";
        case Token::TokenSpecification::TokenType::Or:
            return "Or";
        case Token::TokenSpecification::TokenType::And:
            return "And";
        case Token::TokenSpecification::TokenType::Semicolon:
            return "Semicolon";
        default:
            break;
        }

        return "None";
    }
}

#define LOG_MESSAGE(msg, type) ::Logging::LogMessage((msg), (type))