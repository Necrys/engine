#pragma once

#include <cstring>
#include <cassert>
#include <ctime>
#include <chrono>

namespace engine {

template <typename... Args>
void LoggerIf::debug(const char* format, Args const & ... args) {
    if (m_logger.level() & Logger::Level::debug)
        m_logger.printf("DEBUG", m_name.c_str(), format, args...);
}

template <typename... Args>
void LoggerIf::error(const char* format, Args const & ... args) {
    if (m_logger.level() & Logger::Level::error)
        m_logger.printf("ERROR", m_name.c_str(), format, args...);
}

template <typename... Args>
void LoggerIf::warning(const char* format, Args const &... args) {
    if (m_logger.level() & Logger::Level::warning)
        m_logger.printf("WARN ", m_name.c_str(), format, args...);
}

template <typename... Args>
void LoggerIf::info(const char* format, Args const &... args) {
    if (m_logger.level() & Logger::Level::info)
        m_logger.printf("INFO ", m_name.c_str(), format, args...);
}

template <typename... Args>
void Logger::printf(const char* level, const char* name, const char* format, Args const & ... args) {
    if (!m_file)
        return;

    assert(strlen(format) < 80);

    using namespace std::chrono;

    char ts[20];
    const auto& tp = std::chrono::system_clock::now();
    auto ttime_t = system_clock::to_time_t(tp);
    auto tp_sec = system_clock::from_time_t(ttime_t);
    milliseconds ms = duration_cast<milliseconds>(tp - tp_sec);

    std::tm* ttm = localtime(&ttime_t);

    strftime(ts, sizeof(ts), "%Y.%m.%d|%H-%M-%S", ttm);

    char fmt[128];
    sprintf(fmt, "%s.%03lld|%s%s\n", ts, ms.count(), "%s|%16s| ", format);
    fprintf(m_file, fmt, level, name, args...);
    fflush(m_file);
}

}
