#pragma once

#include "Core/Types.h"
#include <string>
#include <fstream>
#include <iostream>

namespace res {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    
    void setLogLevel(LogLevel level) {
        m_logLevel = level;
    }
    
    void debug(const std::string& message) {
        log(LogLevel::Debug, message);
    }
    
    void info(const std::string& message) {
        log(LogLevel::Info, message);
    }
    
    void warning(const std::string& message) {
        log(LogLevel::Warning, message);
    }
    
    void error(const std::string& message) {
        log(LogLevel::Error, message);
    }
    
private:
    Logger() : m_logLevel(LogLevel::Info) {}
    ~Logger() = default;
    
    void log(LogLevel level, const std::string& message) {
        if (level < m_logLevel) return;
        
        const char* prefix = "";
        switch (level) {
            case LogLevel::Debug: prefix = "[DEBUG]"; break;
            case LogLevel::Info: prefix = "[INFO]"; break;
            case LogLevel::Warning: prefix = "[WARN]"; break;
            case LogLevel::Error: prefix = "[ERROR]"; break;
        }
        
        std::cout << prefix << " " << message << std::endl;
    }
    
    LogLevel m_logLevel;
};

// Удобные макросы для логирования
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_INFO(msg) Logger::getInstance().info(msg)
#define LOG_WARNING(msg) Logger::getInstance().warning(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)

} // namespace res
