#include <texture.h>
#include <glapi.h>
#include <logger.h>
#include <helpers/fileBuffer.h>

#include <GLFW/glfw3.h>
#include <lodepng.h>

namespace engine {
namespace resources {

using namespace gfx;
using namespace gfx::glapi;

static LoggerIf log("RESOURCES");

Texture::Texture():
    m_width(0),
    m_height(0),
    m_format(0) {
    glGenTextures(1, &m_handle);

    OGL_CHECK_ERRORS();
}

Texture::~Texture() {
    if (0 != m_handle) {
        glDeleteTextures(1, &m_handle);
        OGL_CHECK_ERRORS();
    }
}

bool Texture::setData(const uint8_t* pixels,
                      const uint32_t width,
                      const uint32_t height,
                      const uint32_t bpp,
                            bool     genMipmaps) {
    m_width  = width;
    m_height = height;

    GLenum format;

    switch (bpp) {
    case 8:
        m_format = GL_R8;
        format  = GL_RED;
        break;
    case 24:
        m_format = GL_RGB8;
        format  = GL_RGB;
        break;
    case 32:
        m_format = GL_RGBA8;
        format  = GL_RGBA;
        break;
    default:
        log.warning("Unknown image bpp %u", bpp);
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_handle);

    if (false == genMipmaps) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, (const GLvoid*)pixels);

    if (true == genMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    return OGL_CHECK_ERRORS();
}

GLuint Texture::handle() const {
    return m_handle;
}

uint32_t Texture::width() const {
    return m_width;
}

uint32_t Texture::height() const {
    return m_height;
}

TexturePtr Texture::loadFromPNG(const std::string& path, bool genMipmaps) {
    log.debug("loadFromPNG \"%s\"", path.c_str());

    auto buffer = helper::FileBuffer::read(path);
    if (!buffer) {
        return nullptr;
    }

    std::vector<unsigned char> pixels;
    unsigned width, height;

    // seems that we can't get the initial png pixel type, so use 32 bit depth
    // TODO: investigate this issue
    unsigned result = lodepng::decode(pixels, width, height, static_cast<const unsigned char*>(buffer->data()),
        buffer->size(), LCT_RGBA, 8);

    if (0 != result) {
        log.error("Failed to load %s: %s", path.c_str(), lodepng_error_text(result));
        return nullptr;
    }

    TexturePtr texture(new Texture());
    if (false == texture->setData(pixels.data(), width, height, 32)) {
        return nullptr;
    }

    log.debug("loadFromPNG \"%s\" SUCCESS", path.c_str());

    return texture;
}

} //resources
} //engine
