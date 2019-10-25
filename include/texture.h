#pragma once

#include <glapi.h>

#include <memory>
#include <cstdint>

namespace engine {
namespace resources {

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Texture {
public:
    Texture();
    ~Texture();

    static TexturePtr loadFromPNG(const char* path, bool genMipmaps = false);

    bool setData(const uint8_t* pixels,
                 const uint32_t width,
                 const uint32_t height,
                 const uint32_t bpp,
                       bool     genMipmaps = false
    );

    GLuint handle() const;
    uint32_t width() const;
    uint32_t height() const;

private:
    uint32_t m_width;
    uint32_t m_height;
    GLint    m_format;
    GLuint   m_handle;
};

} //resources
} //engine
