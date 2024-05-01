#pragma once

#include "WGL/glcorearb.h"  // download from https://www.khronos.org/registry/OpenGL/api/GL/glcorearb.h
#include "WGL/wglext.h"     // download from https://www.khronos.org/registry/OpenGL/api/GL/wglext.h
// also download https://www.khronos.org/registry/EGL/api/KHR/khrplatform.h and put in "KHR" folder

#define GL_FUNCTIONS(X) \
    X(PFNGLCREATEBUFFERSPROC,            glCreateBuffers            ) \
    X(PFNGLNAMEDBUFFERSTORAGEPROC,       glNamedBufferStorage       ) \
    X(PFNGLBINDVERTEXARRAYPROC,          glBindVertexArray          ) \
    X(PFNGLBINDBUFFERRANGEPROC,          glBindBufferRange          ) \
    X(PFNGLBINDBUFFERBASEPROC,           glBindBufferBase           ) \
    X(PFNGLNAMEDBUFFERSUBDATAEXTPROC,    glNamedBufferSubData       ) \
    X(PFNGLCREATEVERTEXARRAYSPROC,       glCreateVertexArrays       ) \
    X(PFNGLVERTEXARRAYATTRIBBINDINGPROC, glVertexArrayAttribBinding ) \
    X(PFNGLVERTEXARRAYVERTEXBUFFERPROC,  glVertexArrayVertexBuffer  ) \
    X(PFNGLVERTEXARRAYELEMENTBUFFERPROC, glVertexArrayElementBuffer ) \
    X(PFNGLVERTEXARRAYATTRIBFORMATPROC,  glVertexArrayAttribFormat  ) \
    X(PFNGLENABLEVERTEXARRAYATTRIBPROC,  glEnableVertexArrayAttrib  ) \
    X(PFNGLCREATESHADERPROGRAMVPROC,     glCreateShaderProgramv     ) \
    X(PFNGLGETPROGRAMIVPROC,             glGetProgramiv             ) \
    X(PFNGLGETPROGRAMINFOLOGPROC,        glGetProgramInfoLog        ) \
    X(PFNGLGENPROGRAMPIPELINESPROC,      glGenProgramPipelines      ) \
    X(PFNGLUSEPROGRAMSTAGESPROC,         glUseProgramStages         ) \
    X(PFNGLBINDPROGRAMPIPELINEPROC,      glBindProgramPipeline      ) \
    X(PFNGLPROGRAMUNIFORM3FPROC,         glProgramUniform3f         ) \
    X(PFNGLPROGRAMUNIFORM3FVPROC,        glProgramUniform3fv        ) \
    X(PFNGLPROGRAMUNIFORM1IPROC,         glProgramUniform1i         ) \
    X(PFNGLPROGRAMUNIFORM1FPROC,         glProgramUniform1f         ) \
    X(PFNGLPROGRAMUNIFORMMATRIX3FVPROC,  glProgramUniformMatrix3fv  ) \
    X(PFNGLPROGRAMUNIFORMMATRIX2FVPROC,  glProgramUniformMatrix2fv  ) \
    X(PFNGLPROGRAMUNIFORMMATRIX4FVPROC,  glProgramUniformMatrix4fv  ) \
    X(PFNGLBINDTEXTUREUNITPROC,          glBindTextureUnit          ) \
    X(PFNGLGENERATETEXTUREMIPMAPPROC,    glGenerateTextureMipmap    ) \
    X(PFNGLCREATETEXTURESPROC,           glCreateTextures           ) \
    X(PFNGLTEXTUREPARAMETERIPROC,        glTextureParameteri        ) \
    X(PFNGLTEXSTORAGE2DPROC,             glTexStorage2D             ) \
    X(PFNGLTEXTURESTORAGE2DPROC,         glTextureStorage2D         ) \
    X(PFNGLTEXTURESUBIMAGE2DPROC,        glTextureSubImage2D        ) \
    X(PFNGLCLIPCONTROLPROC,              glClipControl              ) \
    X(PFNGLDEBUGMESSAGECALLBACKPROC,     glDebugMessageCallback     )
//  X(PFNGL, gl) \