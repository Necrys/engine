#pragma once

#include <shader.h>
#include <texture.h>
#include <logger.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <memory>

namespace engine {
namespace gfx {

class Device;

class BatchRenderer {
private:
    const uint32_t m_maxVBOVertexCount = 4096;
    const char* m_positionLoc = "position";
    const char* m_colorLoc = "color";
    const char* m_texcoordsLoc = "texcoords";
    const char* m_projectionLoc = "projection";
    const char* m_samplerLoc = "diffuse";

    enum PrimitiveType {
        ePoints    = 1,
        eLines     = 2,
        eTriangles = 3,
    };

public:
    struct Vertex {
        glm::vec3 position;
        uint32_t  color;
        glm::vec2 texcoords;
    };

    struct Triangle {
        Vertex v[3];
    };

    struct Quad {
        Vertex v[4];
    };

public:
    BatchRenderer();
    ~BatchRenderer() = default;

    BatchRenderer(const BatchRenderer&) = delete;
    BatchRenderer& operator= (const BatchRenderer&) = delete;

    bool init(Device& device);
    void deinit();

    void drawPoint(const float x,
                   const float y,
                   const uint32_t color = 0xffffffff,
                   const float z = 1.0f);

    void drawLine(const float x1,
                  const float y1,
                  const float x2,
                  const float y2,
                  const uint32_t color = 0xffffffff,
                  const float z = 1.0f);

    void drawTriangle(const Triangle& triangle,
                      const resources::TexturePtr texture = resources::TexturePtr());

    void drawQuad(const Quad& quad,
                  const resources::TexturePtr texture = resources::TexturePtr());

    void drawQuad(const float x, const float y,
                  const float w, const float h,
                  const float z, const uint32_t color = 0xffffffff);

    void render();
    
    uint32_t getRenderedPrimitivesCount();

private:
    bool setupBuffers();
    bool setupShaders();
    bool setupSamplers();
    bool setupMatrices(const int width, const int height);
    bool genDefaultTexture();

    void onFramebufferResized(const int width, const int height);

    LoggerIf m_log;

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_IBO;

    Vertex* m_VBOData;

    uint32_t m_VBOCurrentPos;

    PrimitiveType m_currPrimitiveType: 8;

    // attributes
    GLint m_positionLocation;
    GLint m_colorLocation;
    GLint m_texcoordLocation;

    // uniforms
    GLint m_projLocation;
    GLint m_textureLocation;

    resources::ProgramPtr m_program;

    glm::mat4 m_projection;

    resources::TexturePtr m_currTexture;
    resources::TexturePtr m_defaultTex;

    GLuint m_pointSampler;
    GLint  m_samplerLocation;

    uint32_t m_primitivesRendered;
};
    
} //gfx
} //engine
