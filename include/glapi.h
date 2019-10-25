#pragma once

#include <GL/glcorearb.h>
#include <GL/glext.h>

#include <cstdint>

#define OGL_CHECK_ERRORS() glapi::checkError(__FILE__, __LINE__)
#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace engine {
namespace gfx {
namespace glapi {

bool init();

extern PFNGLCREATEPROGRAMPROC     glCreateProgram;
extern PFNGLDELETEPROGRAMPROC     glDeleteProgram;
extern PFNGLCREATESHADERPROC      glCreateShader;
extern PFNGLDELETESHADERPROC      glDeleteShader;
extern PFNGLSHADERSOURCEPROC      glShaderSource;
extern PFNGLCOMPILESHADERPROC     glCompileShader;
extern PFNGLGETPROGRAMIVPROC      glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC       glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog;
extern PFNGLATTACHSHADERPROC      glAttachShader;
extern PFNGLLINKPROGRAMPROC       glLinkProgram;
extern PFNGLVALIDATEPROGRAMPROC   glValidateProgram;
extern PFNGLUSEPROGRAMPROC        glUseProgram;
//extern PFNGLBLENDEQUATIONPROC     glBlendEquation;
extern PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;

extern PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC    glBindVertexArray;

extern PFNGLGENBUFFERSPROC    glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBINDBUFFERPROC    glBindBuffer;
extern PFNGLBUFFERDATAPROC    glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
extern PFNGLUNMAPBUFFERPROC   glUnmapBuffer;

extern PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;

extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORMMATRIX3FVPROC   glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC          glUniform1i;
extern PFNGLUNIFORM1FVPROC         glUniform1fv;
extern PFNGLUNIFORM3FVPROC         glUniform3fv;
extern PFNGLUNIFORM4FVPROC         glUniform4fv;

extern PFNGLGENERATEMIPMAPPROC    glGenerateMipmap;
extern PFNGLGENSAMPLERSPROC       glGenSamplers;
extern PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
//extern PFNGLACTIVETEXTUREPROC     glActiveTexture;
extern PFNGLBINDSAMPLERPROC       glBindSampler;
extern PFNGLDELETESAMPLERSPROC    glDeleteSamplers;

extern PFNGLGETINTEGERVPROC glGetIntegerv;
extern PFNGLVIEWPORTPROC glViewport;
extern PFNGLENABLEPROC glEnable;
extern PFNGLDISABLEPROC glDisable;
extern PFNGLBLENDFUNCPROC glBlendFunc;
extern PFNGLBINDTEXTUREPROC glBindTexture;
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLDRAWELEMENTSPROC glDrawElements;
extern PFNGLSCISSORPROC glScissor;
extern PFNGLISENABLEDPROC glIsEnabled;
extern PFNGLDRAWARRAYSPROC glDrawArrays;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;

bool checkError(const char* file, const uint32_t line);

} //glapi
} //gfx
} //engine
