#include <guirenderer.h>
#include <device.h>
#include <glapi.h>

#include "shaders/gui_renderer_vs.h"
#include "shaders/gui_renderer_fs.h"

#include <imgui.h>
#include <glm/ext.hpp>

namespace engine {
namespace gfx {

using namespace glapi;
using namespace resources;

GUIRenderer::GUIRenderer():
    m_log("GUI_RENDERER"),
    m_VBO(0),
    m_VAO(0),
    m_projection(glm::mat4(1.0f)) {
}

bool GUIRenderer::init() {
    m_log.debug("GUIRenderer::init()");

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_Elements);
    glGenBuffers(1, &m_VBO);
    glGenSamplers(1, &m_linearSampler);
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

    if (false == loadFontsTexture()) {
        return false;
    }

    return true;
}

void GUIRenderer::deinit() {
    m_log.debug("GUIRenderer::deinit()");

    if (0 != m_linearSampler)
        glDeleteSamplers(1, &m_linearSampler);

    if (0 != m_VBO)
        glDeleteBuffers(1, &m_VBO);

    if (0 != m_VAO)
        glDeleteVertexArrays(1, &m_VAO);

    OGL_CHECK_ERRORS();
}

void GUIRenderer::render() {
    ImGui::Render();

    ImDrawData* data = ImGui::GetDrawData();
    if (!data || !data->Valid) {
        m_log.warning("ImDrawData is null");
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    const float width = io.DisplaySize.x * io.DisplayFramebufferScale.x;
    const float height = io.DisplaySize.y * io.DisplayFramebufferScale.y;
    data->ScaleClipRects(io.DisplayFramebufferScale);

    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    m_projection = {
        { 2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
        { 0.0f, 0.0f, -1.0f, 0.0f },
        { -1.0f, 1.0f, 0.0f, 1.0f },
    };

    GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
    GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
    GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
    GLint last_blend_src; glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
    GLint last_blend_dst; glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
    GLint last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
    GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(m_VAO);

    glUseProgram(m_program->handle());
    glUniformMatrix4fv(m_projLocation, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniform1i(m_samplerLocation, 0);

    for (int n = 0; n < data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Elements);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

        for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++) {
            glScissor((int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));

            glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
            glBindSampler(0, m_linearSampler);

            glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);

            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    OGL_CHECK_ERRORS();

    glUseProgram(last_program);
    glActiveTexture(last_active_texture);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindVertexArray(last_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFunc(last_blend_src, last_blend_dst);
    if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
}

bool GUIRenderer::setupBuffers() {
    m_log.debug("GUIRenderer::setupBuffers()");

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    return true;
}

bool GUIRenderer::setupShaders() {
    m_log.debug("GUIRenderer::setupShaders()");

    m_program = Program::createFromCode(gui_vs_glsl_str, strlen(gui_vs_glsl_str), gui_fs_glsl_str, strlen(gui_fs_glsl_str));
    if (!m_program) {
        return false;
    }

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // setup vertex attributes
    m_positionLocation = glGetAttribLocation(m_program->handle(), m_positionLoc);
    if (-1 == m_positionLocation) {
        m_log.error("attribute \"%s\" location is not found", m_positionLoc);
        OGL_CHECK_ERRORS();
        return false;
    }

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

    glVertexAttribPointer(m_positionLocation, 2, GL_FLOAT, GL_FALSE,
        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
    glEnableVertexAttribArray(m_positionLocation);

    m_texcoordLocation = glGetAttribLocation(m_program->handle(), m_texcoordsLoc);
    if (-1 == m_texcoordLocation) {
        m_log.error("attribute \"%s\" location is not found", m_texcoordsLoc);
        OGL_CHECK_ERRORS();
        return false;
    }

    glVertexAttribPointer(m_texcoordLocation, 2, GL_FLOAT, GL_FALSE,
        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
    glEnableVertexAttribArray(m_texcoordLocation);
    
    m_colorLocation = glGetAttribLocation(m_program->handle(), m_colorLoc);
    if (-1 == m_colorLocation) {
        m_log.error("attribute \"%s\" location is not found", m_colorLoc);
        OGL_CHECK_ERRORS();
        return false;
    }

    glVertexAttribPointer(m_colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE,
        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
    glEnableVertexAttribArray(m_colorLocation);
#undef OFFSETOF

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
    glBindVertexArray(0);
    glUseProgram(0);

    return OGL_CHECK_ERRORS();
}

bool GUIRenderer::setupSamplers() {
    m_log.debug("BatchRenderer::setupSamplers()");

    glSamplerParameteri(m_linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_linearSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_linearSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return OGL_CHECK_ERRORS();
}

bool GUIRenderer::loadFontsTexture() {
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

    // Upload texture to graphics system
    m_fontsTexture.reset(new Texture());
    if (false == m_fontsTexture->setData(pixels, width, height, 32)) {
        m_log.error("Failed to load font texture");
        return false;
    }

    // Store our identifier
    io.Fonts->TexID = (void *)(intptr_t)m_fontsTexture->handle();

    return OGL_CHECK_ERRORS();
}

} //gfx
} //engine
