#pragma once

#include <string>

namespace engine {

class Logger;
class Config;

class LoggerIf {
public:
    LoggerIf(const std::string& name);

    template <typename... Args>
    void debug(const char* format, Args const & ... args);

    template <typename... Args>
    void error(const char* format, Args const & ... args);

    template <typename... Args>
    void warning(const char* format, Args const & ... args);

    template <typename... Args>
    void info(const char* format, Args const & ... args);

private:
    std::string m_name;
    Logger&     m_logger;
};

class Logger {
public:
    enum Level: uint8_t {
        debug   = 0x01,
        error   = 0x02,
        warning = 0x04,
        info    = 0x08,

        all     = 0x0f,
    };

    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    static Logger& instance();

    template <typename... Args>
    void printf(const char* level, const char* name, const char* format, Args const & ... args);

    bool init(const Config& cfg);
    void level(const uint8_t mask);
    uint8_t level() const;

private:
    Logger();
    ~Logger();

    FILE*   m_file;
    uint8_t m_level;
};

} //engine

#include "logger.hpp"