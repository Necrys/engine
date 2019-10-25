#pragma once

#include <texture.h>
#include <shader.h>
#include <logger.h>

#include <glm/mat4x4.hpp>

namespace engine {
namespace gfx {

class Device;

class GUIRenderer {
    const char* m_positionLoc = "position";
    const char* m_colorLoc = "color";
    const char* m_texcoordsLoc = "texcoords";
    const char* m_projectionLoc = "projection";
    const char* m_samplerLoc = "diffuse";

public:
    GUIRenderer();
    ~GUIRenderer() = default;

    GUIRenderer(const GUIRenderer&) = delete;
    GUIRenderer& operator= (const GUIRenderer&) = delete;

    bool init();
    void deinit();

    void render();

private:
    bool setupBuffers();
    bool setupShaders();
    bool setupSamplers();
    bool loadFontsTexture();
    
    LoggerIf m_log;

    resources::ProgramPtr m_program;
    GLuint m_VBO;
    GLuint m_Elements;
    GLuint m_VAO;

    // attributes
    GLint m_positionLocation;
    GLint m_colorLocation;
    GLint m_texcoordLocation;

    // uniforms
    GLint m_projLocation;
    GLint m_textureLocation;
    GLint m_samplerLocation;

    GLuint m_linearSampler;

    resources::TexturePtr m_fontsTexture;

    glm::mat4 m_projection;
};

} //gfx
} //engine
