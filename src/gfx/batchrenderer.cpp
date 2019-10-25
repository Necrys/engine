#include <batchrenderer.h>
#include <device.h>
#include "shaders/batch_renderer_vs.h"
#include "shaders/batch_renderer_fs.h"

#include <glm/ext.hpp>

namespace engine {
namespace gfx {

// TODO: rewrite with GlDrawElements
    
using namespace glapi;
using namespace resources;

BatchRenderer::BatchRenderer():
    m_log("BATCHRENDERER"),
    m_VBOData(nullptr),
    m_VBOCurrentPos(0),
    m_currPrimitiveType(eTriangles),
    m_positionLocation(-1),
    m_colorLocation(-1),
    m_texcoordLocation(-1),
    m_projLocation(-1),
    m_textureLocation(-1),
    m_samplerLocation(-1) {
}

bool BatchRenderer::init(Device& device) {
    m_log.debug("BatchRenderer::init()");

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);
    glGenSamplers(1, &m_pointSampler);
    OGL_CHECK_ERRORS();

    if (false == setupBuffers()) {
        return false;
    }

    if (false == setupShaders()) {
        return false;
    }

    if (false == setupSamplers()) {
        return false;
    }

    if (false == genDefaultTexture()) {
        return false;
    }

    {
        using namespace std::placeholders;
        device.registerOnResizeCallback(std::bind(&BatchRenderer::onFramebufferResized, this, _1, _2));
    }

    int fbw, fbh;
    device.getFramebufferSize(&fbw, &fbh);
    setupMatrices(fbw, fbh);

    return true;
}

void BatchRenderer::deinit() {
    m_log.debug("%s", __FUNCTION__);

    if (0 != m_pointSampler) {
        glDeleteSamplers(1, &m_pointSampler);
    }

    if (0 != m_IBO) {
        glDeleteBuffers(1, &m_IBO);
    }

    if (0 != m_VBO) {
        glDeleteBuffers(1, &m_VBO);
    }

    if (0 != m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
    }

    delete[] m_VBOData;

    OGL_CHECK_ERRORS();
}

bool BatchRenderer::setupBuffers() {
    m_log.debug("BatchRenderer::setupBuffers()");

    m_VBOData = new Vertex[m_maxVBOVertexCount];

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_maxVBOVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    m_VBOCurrentPos = 0;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    uint32_t indexCount = (m_maxVBOVertexCount / 4) * 6;
    uint16_t* indicies = new uint16_t[indexCount];
    uint32_t vcount = 0;
    for (uint32_t i=0; i<indexCount; i+=6) {
        indicies[i  ] = vcount;
        indicies[i+1] = vcount + 1;
        indicies[i+2] = vcount + 3;
        
        indicies[i+3] = vcount + 3;
        indicies[i+4] = vcount + 1;
        indicies[i+5] = vcount + 2;

        vcount += 4;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint16_t), indicies, GL_STATIC_DRAW);
    delete[] indicies;
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return OGL_CHECK_ERRORS();
}

bool BatchRenderer::setupShaders() {
    m_log.debug("BatchRenderer::setupShaders()");

    m_program = Program::createFromCode(default_vs_glsl_str, strlen(default_vs_glsl_str), default_fs_glsl_str, strlen(default_fs_glsl_str));
    if (!m_program)
        return false;

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    // setup vertex attributes
    m_positionLocation = glGetAttribLocation(m_program->handle(), m_positionLoc);
    if (-1 == m_positionLocation) {
        m_log.error("attribute \"%s\" location is not found", m_positionLoc);
        OGL_CHECK_ERRORS();
        return false;
    }

    glVertexAttribPointer(m_positionLocation, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(m_positionLocation);

    m_colorLocation = glGetAttribLocation(m_program->handle(), m_colorLoc);
    if (-1 == m_colorLocation) {
        m_log.error("attribute \"%s\" location is not found", m_colorLoc);
        OGL_CHECK_ERRORS();
        return false;
    }

    glVertexAttribPointer(m_colorLocation, 4, GL_UNSIGNED_BYTE, GL_FALSE,
        sizeof(Vertex), BUFFER_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(m_colorLocation);

    m_texcoordLocation = glGetAttribLocation(m_program->handle(), m_texcoordsLoc);
    if (-1 == m_texcoordLocation) {
        m_log.error("attribute \"%s\" location is not found", m_texcoordsLoc);
        OGL_CHECK_ERRORS();
        return false;
    }

    glVertexAttribPointer(m_texcoordLocation, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), BUFFER_OFFSET(3 * sizeof(float) + sizeof(uint32_t)));
    glEnableVertexAttribArray(m_texcoordLocation);

    // setup uniforms
    m_projLocation = glGetUniformLocation(m_program->handle(), m_projectionLoc);
    if (-1 == m_projLocation) {
        m_log.error("uniform \"%s\" location is not found", m_texcoordsLoc);
        OGL_CHECK_ERRORS();
        return false;
    }

    m_samplerLocation = glGetUniformLocation(m_program->handle(), m_samplerLoc);
    if (-1 == m_samplerLocation) {
        m_log.error("uniform \"%s\" location is not found", m_samplerLoc);
        OGL_CHECK_ERRORS();
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    return OGL_CHECK_ERRORS();
}

bool BatchRenderer::setupSamplers() {
    m_log.debug("BatchRenderer::setupSamplers()");

    glSamplerParameteri(m_pointSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(m_pointSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(m_pointSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_pointSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return OGL_CHECK_ERRORS();
}

bool BatchRenderer::genDefaultTexture() {
    m_log.debug("BatchRenderer::genDefaultTexture()");

    uint8_t buffer[] = {0xff, 0xff, 0xff, 0xff};
    m_defaultTex.reset(new Texture());
    if (false == m_defaultTex->setData(buffer, 1, 1, 32)) {
        m_log.error("Failed to generate texture");
        return false;
    }

    m_currTexture = m_defaultTex;

    return true;
}

void BatchRenderer::onFramebufferResized(const int width, const int height) {
    setupMatrices(width, height);
}

bool BatchRenderer::setupMatrices(const int width, const int height) {
    m_log.debug("BatchRenderer::setupMatrices(%d, %d)", width, height);
  
    m_projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    glUseProgram(m_program->handle());
    glUniformMatrix4fv(m_projLocation, 1, GL_FALSE, glm::value_ptr(m_projection));

    return OGL_CHECK_ERRORS();
}

void BatchRenderer::drawPoint(const float x,
                              const float y,
                              const uint32_t color,
                              const float z) {
    if (m_currTexture != m_defaultTex) {
        render();
        m_currTexture = m_defaultTex;
    }

    if (ePoints != m_currPrimitiveType) {
        render();
        m_currPrimitiveType = ePoints;
    }

    if (m_VBOCurrentPos + 1 > m_maxVBOVertexCount) {
        render();
    }

    assert(m_VBOData);

    Vertex* ptr = m_VBOData + m_VBOCurrentPos;
    ptr[0] = {glm::vec3(x, y, z), color, glm::vec2(0.0f, 0.0f)};

    m_VBOCurrentPos++;
}

void BatchRenderer::drawLine(const float x1,
                             const float y1,
                             const float x2,
                             const float y2,
                             const uint32_t color,
                             const float z) {
    if (m_currTexture != m_defaultTex) {
        render();
        m_currTexture = m_defaultTex;
    }

    if (eLines != m_currPrimitiveType) {
        render();
        m_currPrimitiveType = eLines;
    }

    if (m_VBOCurrentPos + 2 > m_maxVBOVertexCount) {
        render();
    }

    assert(m_VBOData);

    Vertex* ptr = m_VBOData + m_VBOCurrentPos;
    ptr[0] = {glm::vec3(x1, y1, z), color, glm::vec2(0.0f, 0.0f)};
    ptr[1] = {glm::vec3(x2, y2, z), color, glm::vec2(0.0f, 0.0f)};

    m_VBOCurrentPos += 2;
}

void BatchRenderer::drawTriangle(const Triangle& triangle,
                                 const TexturePtr texture) {
    if (m_currTexture != texture) {
        render();
        m_currTexture = texture ? texture : m_defaultTex;
    }

    if (eTriangles != m_currPrimitiveType) {
        render();
        m_currPrimitiveType = eTriangles;
    }

    if (m_VBOCurrentPos + 3 > m_maxVBOVertexCount) {
        render();
    }

    assert(m_VBOData);

    Vertex* ptr = m_VBOData + m_VBOCurrentPos;

    ptr[0] = triangle.v[0];
    ptr[1] = triangle.v[1];
    ptr[2] = triangle.v[2];

    m_VBOCurrentPos += 3;
}

void BatchRenderer::drawQuad(const Quad& quad,
                             const TexturePtr texture) {
    if (m_currTexture != texture) {
        render();
        m_currTexture = texture ? texture : m_defaultTex;
    }

    if (eTriangles != m_currPrimitiveType) {
        render();
        m_currPrimitiveType = eTriangles;
    }

    if (m_VBOCurrentPos + 4 > m_maxVBOVertexCount) {
        render();
    }

    assert(m_VBOData);

    Vertex* ptr = m_VBOData + m_VBOCurrentPos;

    ptr[0] = quad.v[0];
    ptr[1] = quad.v[1];
    ptr[2] = quad.v[2];
    ptr[3] = quad.v[3];

    m_VBOCurrentPos += 4;
}

void BatchRenderer::drawQuad(const float x, const float y,
                             const float w, const float h,
                             const float z, const uint32_t color) {
    Quad q;
    q.v[0] = { glm::vec3(x,     y,     z), color, glm::vec2(0.0f, 0.0f) };
    q.v[1] = { glm::vec3(x + w, y,     z), color, glm::vec2(0.0f, 0.0f) };
    q.v[2] = { glm::vec3(x + w, y + h, z), color, glm::vec2(0.0f, 0.0f) };
    q.v[3] = { glm::vec3(x,     y + h, z), color, glm::vec2(0.0f, 0.0f) };

    drawQuad(q);
}

void BatchRenderer::render() {
    // TODO: make full gl state save before
    // and restore after the rendering
    
    if (m_VBOCurrentPos == 0) {
        return;
    }

    assert(m_VBOData);

    GLint currentProgram;             glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    GLint currentTexture;             glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
    GLint currentActiveTexture;         glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTexture);
    GLint currentArrayBuffer;         glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentArrayBuffer);
    GLint currentElementArrayBuffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentElementArrayBuffer);
    GLint currentVertexArray;         glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVertexArray);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    // invalidate and refill buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_maxVBOVertexCount * sizeof(Vertex), 0, GL_DYNAMIC_DRAW);
    Vertex* mapped = reinterpret_cast<Vertex*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, m_maxVBOVertexCount * sizeof(Vertex), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
    if (!mapped) {
        m_log.error("Failed to map VBO buffer range");
        OGL_CHECK_ERRORS();
        return;
    }
    std::copy(m_VBOData, m_VBOData + m_VBOCurrentPos, mapped);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    if (currentProgram != (GLint)m_program->handle()) {
        glUseProgram(m_program->handle());
        glUniformMatrix4fv(m_projLocation, 1, GL_FALSE, glm::value_ptr(m_projection));
        glUniform1i(m_samplerLocation, 0);
    }

    if (m_currTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_currTexture->handle());
        glBindSampler(0, m_pointSampler);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    switch (m_currPrimitiveType) {
    case eTriangles:
        glDrawElements(GL_TRIANGLES, (m_VBOCurrentPos / 4) * 6, GL_UNSIGNED_SHORT, 0);
        OGL_CHECK_ERRORS();
        m_primitivesRendered += m_VBOCurrentPos / 3;
        break;
    case eLines:
        glDrawArrays(GL_LINES, 0, m_VBOCurrentPos);
        OGL_CHECK_ERRORS();
        m_primitivesRendered += m_VBOCurrentPos / 2;
        break;
    case ePoints:
        glDrawArrays(GL_POINTS, 0, m_VBOCurrentPos);
        OGL_CHECK_ERRORS();
        m_primitivesRendered += m_VBOCurrentPos;
        break;
    default:
        break;
    }

    m_VBOCurrentPos = 0;

    glUseProgram(currentProgram);
    glActiveTexture(currentActiveTexture);
    glBindTexture(GL_TEXTURE_2D, currentTexture);
    glBindVertexArray(currentVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, currentArrayBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentElementArrayBuffer);

    OGL_CHECK_ERRORS();
}

uint32_t BatchRenderer::getRenderedPrimitivesCount() {
    uint32_t res = m_primitivesRendered;
    m_primitivesRendered = 0;
    return res;
}

} //gfx
} //engine
