#include <shader.h>
#include <logger.h>
#include <helpers/fileBuffer.h>

namespace engine {
namespace resources {

using namespace gfx;
using namespace gfx::glapi;

static LoggerIf log("RESOURCES");

static const char* shaderTypeToString(const GLenum type) {
    switch (type) {
    case GL_VERTEX_SHADER: return "Vertex Shader";
    case GL_FRAGMENT_SHADER: return "Fragment Shader";
    default: return "Unknown";
    }
}

Shader::Shader(const GLenum type) {
    m_handle = glCreateShader(type);
    OGL_CHECK_ERRORS();
}

Shader::~Shader() {
    if (0 != m_handle) {
        glDeleteShader(m_handle);
        OGL_CHECK_ERRORS();
    }
}

ShaderPtr Shader::create(const char* path, const GLenum type) {
    log.debug("Shader create \"%s\", type: %s", path, shaderTypeToString(type));

    auto buffer = helper::FileBuffer::read(path);
    if (!buffer) {
        return nullptr;
    }

    return createFromCode(reinterpret_cast<const char*>(buffer->data()), buffer->size(), type);
}

ShaderPtr Shader::createFromCode(const char* code, const GLint len, const GLenum type) {
    log.debug("Create %s shader from code\n\n%s\n\n", shaderTypeToString(type), code);

    ShaderPtr shader(new Shader(type));
    
    glShaderSource(shader->handle(), 1, (const GLchar**)&code, &len);
    glCompileShader(shader->handle());
    if (false == shader->checkStatus(GL_COMPILE_STATUS)) {
        return nullptr;
    }

    OGL_CHECK_ERRORS();

    log.debug("Shader create SUCCESS");

    return shader;
}

GLuint Shader::handle() const {
    return m_handle;
}

bool Shader::checkStatus(const GLenum param) {
    GLint status;

    glGetShaderiv(m_handle, param, &status);

    if (GL_TRUE != status) {
        static const uint32_t bufferLen = 1024;
        GLint length;
        GLchar buffer[bufferLen];
        glGetShaderInfoLog(m_handle, bufferLen, &length, buffer);
        log.error("Shader status: %s", (const char*)buffer);
    }

    return status;
}

Program::Program() {
    m_handle = glCreateProgram();
    OGL_CHECK_ERRORS();
}

Program::~Program() {
    if (0 != m_handle) {
        glDeleteProgram(m_handle);
        OGL_CHECK_ERRORS();
    }
}

ProgramPtr Program::create(const char* vsPath, const char* fsPath) {
    log.debug("Program create vs:\"%s\", fs: \"%s\"", vsPath, fsPath);

    ShaderPtr vs = Shader::create(vsPath, GL_VERTEX_SHADER);
    if (!vs) {
        OGL_CHECK_ERRORS();
        return nullptr;
    }

    ProgramPtr program(new Program());

    glAttachShader(program->handle(), vs->handle());

    ShaderPtr fs = Shader::create(fsPath, GL_FRAGMENT_SHADER);
    if (!fs) {
        OGL_CHECK_ERRORS();
        return nullptr;
    }

    glAttachShader(program->handle(), fs->handle());
    glLinkProgram(program->handle());

    if (false == program->checkStatus(GL_LINK_STATUS)) {
        OGL_CHECK_ERRORS();
        return nullptr;
    }

    glValidateProgram(program->handle());

    OGL_CHECK_ERRORS();

    log.debug("Program create vs:\"%s\", fs: \"%s\", SUCCESS", vsPath, fsPath);

    return program;
}

ProgramPtr Program::createFromCode(const char* vsCode, const GLint vsLen, const char* fsCode, const GLint fsLen) {
    log.debug("Program create from code\n\nvs:\n\n%s\n\nfs:\n\n%s\n", vsCode, fsCode);

    ShaderPtr vs = Shader::createFromCode(vsCode, vsLen, GL_VERTEX_SHADER);
    if (!vs) {
        OGL_CHECK_ERRORS();
        return nullptr;
    }

    ProgramPtr program(new Program());

    glAttachShader(program->handle(), vs->handle());

    ShaderPtr fs = Shader::createFromCode(fsCode, fsLen, GL_FRAGMENT_SHADER);
    if (!fs) {
        OGL_CHECK_ERRORS();
        return nullptr;
    }

    glAttachShader(program->handle(), fs->handle());
    glLinkProgram(program->handle());

    if (false == program->checkStatus(GL_LINK_STATUS)) {
        OGL_CHECK_ERRORS();
        return nullptr;
    }

    glValidateProgram(program->handle());

    OGL_CHECK_ERRORS();

    log.debug("Program create SUCCESS");

    return program;
}

GLuint Program::handle() const {
    return m_handle;
}

bool Program::checkStatus(const GLenum param) {
    GLint  status;

    glGetProgramiv(m_handle, param, &status);

    if (GL_TRUE != status) {
        static const uint32_t bufferLen = 1024;
        GLint length;
        GLchar buffer[bufferLen];
        glGetProgramInfoLog(m_handle, bufferLen, &length, buffer);
        log.error("Program status: %s", (const char *)buffer);
    }

    return status;
}

} //resources
} //engine
