#include <glapi.h>
#include <logger.h>

#include <GLFW/glfw3.h>

#define GL_API_GET_FUNCTION_PTR(p,n) \
    n = (p)glfwGetProcAddress(#n); \
    if (nullptr == n) { \
        log.error("\"%s\" is not supported", #n); \
        return false; \
    }

namespace engine {
namespace gfx {
namespace glapi {

PFNGLCREATEPROGRAMPROC     glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC     glDeleteProgram = nullptr;
PFNGLCREATESHADERPROC      glCreateShader = nullptr;
PFNGLDELETESHADERPROC      glDeleteShader = nullptr;
PFNGLSHADERSOURCEPROC      glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC     glCompileShader = nullptr;
PFNGLGETPROGRAMIVPROC      glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGETSHADERIVPROC       glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog = nullptr;
PFNGLATTACHSHADERPROC      glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC       glLinkProgram = nullptr;
PFNGLVALIDATEPROGRAMPROC   glValidateProgram = nullptr;
PFNGLUSEPROGRAMPROC        glUseProgram = nullptr;
PFNGLBLENDEQUATIONPROC     glBlendEquation = nullptr;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = nullptr;

PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC    glBindVertexArray = nullptr;

PFNGLGENBUFFERSPROC    glGenBuffers = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLBINDBUFFERPROC    glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC    glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange = nullptr;
PFNGLUNMAPBUFFERPROC   glUnmapBuffer = nullptr;

PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray = nullptr;
        
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORMMATRIX3FVPROC   glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv = nullptr;
PFNGLUNIFORM1IPROC          glUniform1i = nullptr;
PFNGLUNIFORM1FVPROC         glUniform1fv = nullptr;
PFNGLUNIFORM3FVPROC         glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC         glUniform4fv = nullptr;

PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;
PFNGLGENSAMPLERSPROC glGenSamplers = nullptr;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLBINDSAMPLERPROC glBindSampler = nullptr;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = nullptr;

PFNGLGETINTEGERVPROC glGetIntegerv = nullptr;
PFNGLVIEWPORTPROC glViewport = nullptr;
PFNGLENABLEPROC glEnable = nullptr;
PFNGLDISABLEPROC glDisable = nullptr;
PFNGLBLENDFUNCPROC glBlendFunc = nullptr;
PFNGLBINDTEXTUREPROC glBindTexture = nullptr;
//PFNGLBLENDEQUATIONPROC glBlendEquation = nullptr;
//PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLDRAWELEMENTSPROC glDrawElements = nullptr;
PFNGLSCISSORPROC glScissor = nullptr;
PFNGLISENABLEDPROC glIsEnabled = nullptr;
PFNGLDRAWARRAYSPROC glDrawArrays = nullptr;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = nullptr;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange = nullptr;

static LoggerIf log("OGL");

void APIENTRY MessageCallback(GLenum source,
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei length,
                              const GLchar* message,
                              const void* userParam)
{
    log.debug("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

bool init() {
    GL_API_GET_FUNCTION_PTR(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    GL_API_GET_FUNCTION_PTR(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
    GL_API_GET_FUNCTION_PTR(PFNGLCREATESHADERPROC, glCreateShader);
    GL_API_GET_FUNCTION_PTR(PFNGLDELETESHADERPROC, glDeleteShader);
    GL_API_GET_FUNCTION_PTR(PFNGLSHADERSOURCEPROC, glShaderSource);
    GL_API_GET_FUNCTION_PTR(PFNGLCOMPILESHADERPROC, glCompileShader);
    GL_API_GET_FUNCTION_PTR(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
    GL_API_GET_FUNCTION_PTR(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
    GL_API_GET_FUNCTION_PTR(PFNGLGETSHADERIVPROC, glGetShaderiv);
    GL_API_GET_FUNCTION_PTR(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    GL_API_GET_FUNCTION_PTR(PFNGLATTACHSHADERPROC, glAttachShader);
    GL_API_GET_FUNCTION_PTR(PFNGLLINKPROGRAMPROC, glLinkProgram);
    GL_API_GET_FUNCTION_PTR(PFNGLVALIDATEPROGRAMPROC, glValidateProgram);
    GL_API_GET_FUNCTION_PTR(PFNGLUSEPROGRAMPROC, glUseProgram);
    GL_API_GET_FUNCTION_PTR(PFNGLBLENDEQUATIONPROC, glBlendEquation);
    GL_API_GET_FUNCTION_PTR(PFNGLBLENDEQUATIONSEPARATEPROC, glBlendEquationSeparate);

    GL_API_GET_FUNCTION_PTR(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
    GL_API_GET_FUNCTION_PTR(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
    GL_API_GET_FUNCTION_PTR(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);

    GL_API_GET_FUNCTION_PTR(PFNGLGENBUFFERSPROC, glGenBuffers);
    GL_API_GET_FUNCTION_PTR(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
    GL_API_GET_FUNCTION_PTR(PFNGLBINDBUFFERPROC, glBindBuffer);
    GL_API_GET_FUNCTION_PTR(PFNGLBUFFERDATAPROC, glBufferData);
    GL_API_GET_FUNCTION_PTR(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
    GL_API_GET_FUNCTION_PTR(PFNGLMAPBUFFERRANGEPROC, glMapBufferRange);
    GL_API_GET_FUNCTION_PTR(PFNGLUNMAPBUFFERPROC, glUnmapBuffer);

    GL_API_GET_FUNCTION_PTR(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
    GL_API_GET_FUNCTION_PTR(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
    GL_API_GET_FUNCTION_PTR(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);

    GL_API_GET_FUNCTION_PTR(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
    GL_API_GET_FUNCTION_PTR(PFNGLUNIFORMMATRIX3FVPROC,   glUniformMatrix3fv);
    GL_API_GET_FUNCTION_PTR(PFNGLUNIFORMMATRIX4FVPROC,   glUniformMatrix4fv);
    GL_API_GET_FUNCTION_PTR(PFNGLUNIFORM1IPROC,          glUniform1i);
    GL_API_GET_FUNCTION_PTR(PFNGLUNIFORM1FVPROC,         glUniform1fv);
    GL_API_GET_FUNCTION_PTR(PFNGLUNIFORM3FVPROC,         glUniform3fv);
    GL_API_GET_FUNCTION_PTR(PFNGLUNIFORM4FVPROC,         glUniform4fv);

    GL_API_GET_FUNCTION_PTR(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap);
    GL_API_GET_FUNCTION_PTR(PFNGLGENSAMPLERSPROC, glGenSamplers);
    GL_API_GET_FUNCTION_PTR(PFNGLSAMPLERPARAMETERIPROC, glSamplerParameteri);
    GL_API_GET_FUNCTION_PTR(PFNGLACTIVETEXTUREPROC, glActiveTexture);
    GL_API_GET_FUNCTION_PTR(PFNGLBINDSAMPLERPROC, glBindSampler);
    GL_API_GET_FUNCTION_PTR(PFNGLDELETESAMPLERSPROC, glDeleteSamplers);

    GL_API_GET_FUNCTION_PTR(PFNGLGETINTEGERVPROC, glGetIntegerv);
    GL_API_GET_FUNCTION_PTR(PFNGLVIEWPORTPROC, glViewport);
    GL_API_GET_FUNCTION_PTR(PFNGLENABLEPROC, glEnable);
    GL_API_GET_FUNCTION_PTR(PFNGLDISABLEPROC, glDisable);
    GL_API_GET_FUNCTION_PTR(PFNGLBLENDFUNCPROC, glBlendFunc);
    GL_API_GET_FUNCTION_PTR(PFNGLBINDTEXTUREPROC, glBindTexture);
//    GL_API_GET_FUNCTION_PTR(PFNGLBLENDEQUATIONPROC, glBlendEquation);
//    GL_API_GET_FUNCTION_PTR(PFNGLACTIVETEXTUREPROC, glActiveTexture);
    GL_API_GET_FUNCTION_PTR(PFNGLDRAWELEMENTSPROC, glDrawElements);
    GL_API_GET_FUNCTION_PTR(PFNGLSCISSORPROC, glScissor);
    GL_API_GET_FUNCTION_PTR(PFNGLISENABLEDPROC, glIsEnabled);
    GL_API_GET_FUNCTION_PTR(PFNGLDRAWARRAYSPROC, glDrawArrays);
    GL_API_GET_FUNCTION_PTR(PFNGLFLUSHMAPPEDBUFFERRANGEPROC, glFlushMappedBufferRange);

    GL_API_GET_FUNCTION_PTR(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    return true;
}

bool checkError(const char* file, const uint32_t line) {
    GLenum glErrno = glGetError();
    if (GL_NO_ERROR != glErrno) {
        log.error("(%s: %u) glGetError: %u\n", file, line, glErrno);
        return false;
    }

    return true;
}

} //glapi
} //gfx
} //engine