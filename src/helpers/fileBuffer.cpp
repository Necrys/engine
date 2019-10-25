#include "fileBuffer.h"

#include <cstdio>
#include <cerrno>
#include <cstring>

namespace engine {
namespace helper {

LoggerIf FileBuffer::m_log("FILE_BUFFER");

FileBuffer::FileBuffer(uint8_t* data, const size_t size):
    m_data (data),
    m_size (size) {
}

FileBuffer::~FileBuffer() {
    if (m_data) {
        delete[] m_data;
    }
}

FileBufferPtr FileBuffer::read(const std::string& path) {
    m_log.debug("FileBuffer::read(%s)", path.c_str());

    FileBufferPtr fbuf;

    FILE* stream = fopen(path.c_str(), "rb");
    if (!stream) {
        m_log.error("Unable to open \"%s\" for reading, code %d (%s)", path.c_str(), errno, strerror(errno));
        return fbuf;
    }

    if (0 != fseek (stream, 0L, SEEK_END)) {
        int err = ferror (stream);
        m_log.error("Failed to seek file \"%s\": %d\n", path.c_str(), err);
        fclose (stream);
        return fbuf;
    }

    long int size = ftell (stream);
    rewind (stream);

    if (size <= 0) {
        m_log.error("Incorrect file size \"%s\": %ld\n", path.c_str(), size);
        fclose (stream);
        return fbuf;
    }

    uint8_t* buffer = new uint8_t[size];
    size_t rres = fread ((void*)buffer, sizeof(uint8_t), size, stream);
    if ((long int)rres != size) {
        m_log.error("Failed to read file \"%s\": size: %ld, read: %lu\n", path.c_str(), size, rres);
        delete[] buffer;
        fclose (stream);
        return fbuf;
    }

    fclose (stream);
    fbuf.reset(new FileBuffer(buffer, size));

    return fbuf;
}

const uint8_t* FileBuffer::data() const {
    return m_data;
}

uint8_t* FileBuffer::data() {
    return m_data;
}

uint32_t FileBuffer::size() const {
    return m_size;
}


} //helper
} //engine