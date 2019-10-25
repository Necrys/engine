#pragma once

#include <logger.h>

#include <memory>

namespace engine {
namespace helper {

class FileBuffer;
using FileBufferPtr = std::shared_ptr<FileBuffer>;

class FileBuffer {
public:
    ~FileBuffer();

    FileBuffer(const FileBuffer&) = delete;
    FileBuffer& operator=(const FileBuffer&) = delete;

    static FileBufferPtr read(const std::string& path);

    const uint8_t* data() const;
    uint8_t* data();
    uint32_t size() const;

private:
    FileBuffer(uint8_t* data, const size_t size);

    uint8_t* m_data;
    size_t   m_size;

    static LoggerIf m_log;
};

} //helper
} //engine