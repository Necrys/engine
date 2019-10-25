#include <logger.h>
#include <config.h>

namespace engine {

LoggerIf::LoggerIf(const std::string& name):
    m_name(name),
    m_logger(Logger::instance()) { // god save us...
    
}

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

Logger::Logger():
    m_file(nullptr),
    m_level(Level::all) {
}

Logger::~Logger() {
    if (m_file)
        fclose(m_file);
}

bool Logger::init(const Config& cfg) {
    std::string path = "log.txt";
    cfg.getString("logger", "file", path);

    m_file = fopen(path.c_str(), "w");
    if (!m_file)
        return false;

    int32_t val = 0;

    if (cfg.getInt("logger", "debug", &val)) {
        if (val)
            m_level |= Level::debug;
        else
            m_level = m_level & ~Level::debug;
    }

    val = 0;
    if (cfg.getInt("logger", "error", &val)) {
        if (val)
            m_level |= Level::error;
        else
            m_level = m_level & ~Level::error;
    }

    val = 0;
    if (cfg.getInt("logger", "warning", &val)) {
        if (val)
            m_level |= Level::warning;
        else
            m_level = m_level & ~Level::warning;
    }

    val = 0;
    if (cfg.getInt("logger", "info", &val)) {
        if (val)
            m_level |= Level::info;
        else
            m_level = m_level & ~Level::info;
    }

    return true;
}

void Logger::level(const uint8_t mask) {
    m_level = mask;
}

uint8_t Logger::level() const {
    return m_level;
}

}
