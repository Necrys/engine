#pragma once

#include <glapi.h>

#include <memory>

namespace engine {
namespace resources {

class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

class Program;
typedef std::shared_ptr<Program> ProgramPtr;

class Shader {
public:
    Shader(const GLenum type);
    ~Shader();

    static ShaderPtr create(const char* path, const GLenum type);
    static ShaderPtr createFromCode(const char* code, const GLint len, const GLenum type);

    GLuint handle() const;

private:
    bool checkStatus(const GLenum param);

    GLuint m_handle;
};

class Program {
public:
    Program();
    ~Program();

    static ProgramPtr create(const char* vsPath, const char* fsPath);
    static ProgramPtr createFromCode(const char* vsCode, const GLint vsLen, const char* fsCode, const GLint fsLen);

    GLuint handle() const;
private:
    bool checkStatus(const GLenum param);

    GLuint m_handle;
};

} //resources
} //engine
