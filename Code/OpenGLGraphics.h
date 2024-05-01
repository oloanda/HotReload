#pragma once
#include "OpenGLFunctions.h"


struct Shader
{
    unsigned int pipeline, vertex, fragment;
    const char* vertexPath;
    const char* fragmentPath;
};


extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X