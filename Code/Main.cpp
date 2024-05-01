#define _CRT_SECURE_NO_WARNINGS
#include "Platform.h"

#include "Model.h"
#include <GL/gl.h>

#include "Camera.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



#if _DEBUG
#pragma comment(linker,"/SUBSYSTEM:CONSOLE")
#else 
#pragma comment(linker,"/SUBSYSTEM:WINDOWS")
#endif
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "opengl32.lib")

static bool Running = false;
static int gScreenWidth = 1024;
static int gScreenHeight = 768;
static bool wasScreenShot = false;
static bool screenShot = false;
static int frameCounter = 0;


static void FatalError(const char* message)
{
    MessageBoxA(NULL,message, "Error", MB_ICONEXCLAMATION);
    ExitProcess(0);
}

#ifndef NDEBUG
static void APIENTRY DebugCallback(
    GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* user)
{
    printf(message);
    printf("\n");
    if(severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
    {
        if(IsDebuggerPresent())
        {
            Assert(!"OpenGL error - check the callstack in debugger");
        }
        FatalError("OpenGL API usage error ! Use debugger to examine call stack !");
    }
}
#endif


static LRESULT CALLBACK MainWindow32Callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    switch (message)
    {
        case WM_QUIT:
        {
            Running = false;
            DestroyWindow(window);
            //continue;
        }
        break;
        case WM_DESTROY:
        {
            Running = false;
            PostQuitMessage(0);
        }
        case WM_CLOSE: 
        {
            Running = false;

        }
		break;
        default:
        {
            result = DefWindowProcW(window, message, wparam, lparam);
        }
        break;
        
    }
    return result;
}


static int StringsAreEqual(const char* src, const char* dst, size_t dstlen)
{
    while(*src && dstlen-- && *dst)
    {
        if(*src++ != *dst++)
        {
            return 0;
        }
    }

    return (dstlen && *src == *dst) || (!dstlen && *src == 0);
}

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

#define X(type, name) type name;
GL_FUNCTIONS(X)
#undef X

static void GetWglFunctions()
{
    HWND dummy = CreateWindowExW(0,L"STATIC", L"DummyWindow",
    WS_OVERLAPPED, CW_USEDEFAULT,CW_USEDEFAULT,
    CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL, NULL, NULL);

    Assert(dummy && "Failed to create dummy window");

    HDC dc = GetDC(dummy);
    Assert(dc && "Failed to get device context for dummy window");


    PIXELFORMATDESCRIPTOR desc = 
    {
        sizeof(desc),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24
    };


    int format = ChoosePixelFormat(dc, &desc);
    if(!format)
    {
        FatalError("Cannot choose OpenGL pixel format for dummy window !");
    }

    int ok = DescribePixelFormat(dc,format, sizeof(desc),&desc);
    Assert(ok && "Failed to describe OpenGL pixel format");

    if(!SetPixelFormat(dc, format,&desc))
    {
        FatalError("Cannot set OpenGL pixel foramt for dummy window");
    }


    HGLRC rc = wglCreateContext(dc);
    Assert(rc && "Failed to create OpenGL context for dummy window");

    ok = wglMakeCurrent(dc, rc);

    // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_extensions_string.txt
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
        (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
    if (!wglGetExtensionsStringARB)
    {
        FatalError("OpenGL does not support WGL_ARB_extensions_string extension!");
    }

    const char* ext = wglGetExtensionsStringARB(dc);
    Assert(ext && "Failed to get OpenGL WGL extension string");


    const char* start = ext;
    for(;;)
    {
        while(*ext != 0 && *ext != ' ')
        {
            ext++;
        }

        size_t length = ext - start;
        if (StringsAreEqual("WGL_ARB_pixel_format", start, length))
        {
            // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
            wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
        }
        else if (StringsAreEqual("WGL_ARB_create_context", start, length))
        {
            // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
            wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
        }
        else if (StringsAreEqual("WGL_EXT_swap_control", start, length))
        {
            // https://www.khronos.org/registry/OpenGL/extensions/EXT/WGL_EXT_swap_control.txt
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        }

        if(*ext == 0)
        {
            break;
        }

        ext++;
        start = ext;

    }

    if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB || !wglSwapIntervalEXT)
    {
        FatalError("OpenGL does not support required WGL extensions for modern context!");
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(rc);
    ReleaseDC(dummy,dc);
    DestroyWindow(dummy);


}

#if 0
void ShowFPS(GLFWwindow* window)
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime();

    elapsedSeconds = currentSeconds - previousSeconds;


    if (elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed
            << APP_TITLE << "    "
            << "FPS: " << fps << "    "
            << "Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }

    frameCount++;
}
#endif

static void InitWin32OpenGL(HWND window)
{
    // set pixel format for OpenGL context
    
    int attrib[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,     24,
        WGL_DEPTH_BITS_ARB,     GL_DEPTH_COMPONENT32F,
        WGL_STENCIL_BITS_ARB,   8,

        // uncomment for sRGB framebuffer, from WGL_ARB_framebuffer_sRGB extension
        // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_framebuffer_sRGB.txt
        WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,

        // uncomment for multisampeld framebuffer, from WGL_ARB_multisample extension
        // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_multisample.txt
        WGL_SAMPLE_BUFFERS_ARB, 16,
        WGL_SAMPLES_ARB,        4, // 4x MSAA

        0,
    };



    HDC dc = GetDC(window);

    int format;
    UINT formats;
    if (!wglChoosePixelFormatARB(dc, attrib, NULL, 1, &format, &formats) || formats == 0)
    {
        FatalError("OpenGL does not support required pixel format!");
    }

    PIXELFORMATDESCRIPTOR desc = {sizeof(desc) };
    int ok = DescribePixelFormat(dc, format, sizeof(desc), &desc);
    Assert(ok && "Failed to describe OpenGL pixel format");

    if (!SetPixelFormat(dc, format, &desc))
    {
        FatalError("Cannot set OpenGL selected pixel format!");
    }
    

    // create modern OpenGL context
    {
        int attrib[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifndef NDEBUG
            // ask for debug context for non "Release" builds
            // this is so we can enable debug callback
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
            0,
        };

        HGLRC rc = wglCreateContextAttribsARB(dc, NULL, attrib);
        if (!rc)
        {
            FatalError("Cannot create modern OpenGL context! OpenGL version 4.5 not supported?");
        }

        BOOL ok = wglMakeCurrent(dc, rc);
        Assert(ok && "Failed to make current OpenGL context");

        // load OpenGL functions
#define X(type, name) name = (type)wglGetProcAddress(#name); Assert(name);
        GL_FUNCTIONS(X)
#undef X

#ifndef NDEBUG
        // enable debug callback
        glDebugMessageCallback(&DebugCallback, NULL);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
    }

    // setup global GL state
    {
        // enable alpha blending
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_GREATER);
        //glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
        //glClearDepth(0.0);

        // disable culling
        glDisable(GL_CULL_FACE);
    }
}

static void CompileShader(Memory* memory ,Shader* shader)
{

    FileResource vertexFile(shader->vertexPath, memory);
    LoadFile(vertexFile, memory, false, nullptr);
    char* vertexSource = (char *)vertexFile.data;

    FileResource fragmentFile(shader->fragmentPath, memory);
    LoadFile(fragmentFile, memory, false, nullptr);
    char* fragmentSource = (char *)fragmentFile.data;


    GLint linked;
    bool success = true;

    shader->vertex = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertexSource);
    glGetProgramiv(shader->vertex, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        char message[1024] = {};
        glGetProgramInfoLog( shader->vertex, 1024, nullptr, message);
        printf("%s : \n", message);
        Assert(!"Failed to create vertex shader !!!");
        success = false;
    }

    shader->fragment = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragmentSource);
    glGetProgramiv(shader->fragment, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        char message[1024] = {};
        glGetProgramInfoLog( shader->fragment, 1024, nullptr, message);
        printf("%s : \n", message);
        Assert(!"Failed to create fragment shader !!!");
        success = false;
    }

    if(success)
    {
        glGenProgramPipelines(1, &shader->pipeline);
        glUseProgramStages(shader->pipeline, GL_VERTEX_SHADER_BIT, shader->vertex);
        glUseProgramStages(shader->pipeline, GL_FRAGMENT_SHADER_BIT, shader->fragment);
    }

}

void DrawGrid(Model& model, Shader* shader, Camera& camera)
{


    for(int i = 0; i < model.meshCount; i++)
    {
        glBindProgramPipeline(shader->pipeline);
        glBindVertexArray(model.meshes[i].VAO);

        glProgramUniformMatrix4fv(shader->vertex, 0, 1, GL_TRUE,mat4().asArray);
        glProgramUniformMatrix4fv(shader->vertex, 1, 1, GL_TRUE,camera.view.asArray);
        glProgramUniformMatrix4fv(shader->vertex, 2, 1, GL_TRUE, camera.projection.asArray);

        glDrawElements(GL_LINES, model.meshes[i].indexCount, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_TRIANGLES, model.meshes[i].indexCount, GL_UNSIGNED_INT, nullptr);

        glBindProgramPipeline(0);


    }
}

void DrawModel(Model& model,Shader* shader, Camera& camera)
{
    for(int i = 0; i < model.meshCount; i++)
    {

        glBindProgramPipeline(shader->pipeline);

        glBindVertexArray(model.meshes[i].VAO);
        glProgramUniformMatrix4fv(shader->vertex, 0, 1, GL_TRUE,model.modelMatrix.asArray);


        if(model.meshes[i].material.diffuseMap.index == 0)
        {
            glProgramUniform1i(shader->fragment, 0, 1);
        }
        else
        {
            glBindTextureUnit(0, model.meshes[i].material.diffuseMap.index);
            glProgramUniform1i(shader->fragment, 0, 0);

        }


        GLint sDiffuse = 1;
        glProgramUniform3fv(shader->fragment, sDiffuse, 1, model.meshes[i].material.diffuse.asArray);

        glDrawElements(GL_TRIANGLES, model.meshes[i].indexCount, GL_UNSIGNED_INT, nullptr);

    }
}

static void SaveScreenShot(Memory* memory, WindowDimension windowDimension)
{
    static size_t count = 1;
    char tempBufffer[128];
    //snprintf(tempBufffer, 128, "screeshot%02zX.png", count);
    snprintf(tempBufffer, 128, "screeshot%02zd.png", count);
    count++;

    int width, height;
    width = windowDimension.width;
    height = windowDimension.height;

    stbi_flip_vertically_on_write(true);
    size_t used = memory->tempUsedSize;
    unsigned char* data = (unsigned char *)MyTempAlloc(memory, width * height * 4);
    glReadPixels(0,0,width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_write_png(tempBufffer, width, height, 4,data, 0);

    memory->tempUsedSize = used;

    
}

static void UpdateCamera(Camera& camera,WindowDimension windowDimension,Input& input ,float deltaTime)
{
    float speed = deltaTime * 2.5f;
    if(camera.input.moveForward)
    {
        camera.acceleration.x += camera.view.forward.x * speed;
        camera.acceleration.y += camera.view.forward.y * speed;
        camera.acceleration.z += camera.view.forward.z * speed;
    }

    if(camera.input.moveBackward)
    {
        camera.acceleration.x += -camera.view.forward.x * speed;
        camera.acceleration.y += -camera.view.forward.y * speed;
        camera.acceleration.z += -camera.view.forward.z * speed;
    }

    if(camera.input.moveUp)
    {
        vec3 dir = vec3(0, 1, 0);
        Normalize(dir);

        camera.acceleration.x += dir.x * speed;
        camera.acceleration.y += dir.y * speed;
        camera.acceleration.z += dir.z * speed;
    }

    if(camera.input.moveDown)
    {
        vec3 dir = vec3(0, -1, 0);
        Normalize(dir);
        camera.acceleration.x += dir.x * speed;
        camera.acceleration.y += dir.y * speed;
        camera.acceleration.z += dir.z * speed;
    }

    if(camera.input.moveRight)
    {
        camera.acceleration.x += camera.view.right.x * speed;
        camera.acceleration.y += camera.view.right.y * speed;
        camera.acceleration.z += camera.view.right.z * speed;
    }

    if(camera.input.moveLeft)
    {
        camera.acceleration.x += -camera.view.right.x * speed;
        camera.acceleration.y += -camera.view.right.y * speed;
        camera.acceleration.z += -camera.view.right.z * speed;
    }

    if(camera.input.rotateCamera)
    {
        camera.rotation.x += 2.9f * input.mouseDeltaY * deltaTime;
        camera.rotation.y += 2.9f * input.mouseDeltaX * deltaTime;

        camera.rotation.x = CorrectDegree(camera.rotation.x);
        camera.rotation.y = CorrectDegree(camera.rotation.y);
    }
    
    camera.position += camera.acceleration;
    camera.acceleration *= 0.9f;

    camera.view = Transform(vec3(1, -1, 1), vec3(camera.rotation.x, camera.rotation.y, 0), camera.position);
    camera.view = FastInverse(camera.view);
    camera.projection = Projection(45.0f, (float)gScreenWidth / (float)gScreenHeight, 0.1f, 1000.0f);

    //camera.view = FastInverse(camera.view);
    //camera.projection = Transpose(Projection(60.0f, (float)windowDimension.width / (float)windowDimension.height, 0.1f, 1000.0f));
    camera.viewProjection = camera.view * camera.projection;


}

static void ProcessInput(Input& input,WindowDimension windowDimension,Memory* memory )
{
    if(input.F && !wasScreenShot)
    {
        if(screenShot)
        {
            SaveScreenShot(memory, windowDimension);
        }

        screenShot = !screenShot;
    }
    wasScreenShot = input.F;
}

void  InitSkybox(Skybox& skybox)
{
    const int vertexCount = 8;
    const int indexCount = 36;
    
    
    vec3 vertices[vertexCount];
    vertices[0]=vec3(-0.5f,-0.5f,-0.5f);
    vertices[1]=vec3( 0.5f,-0.5f,-0.5f);
    vertices[2]=vec3( 0.5f, 0.5f,-0.5f);
    vertices[3]=vec3(-0.5f, 0.5f,-0.5f);
    vertices[4]=vec3(-0.5f,-0.5f, 0.5f);
    vertices[5]=vec3( 0.5f,-0.5f, 0.5f);
    vertices[6]=vec3( 0.5f, 0.5f, 0.5f);
    vertices[7]=vec3(-0.5f, 0.5f, 0.5f); 
    

    unsigned int indices[indexCount];
    unsigned int trianglesIndices[36] = 
    {
        //bottom face
        0, 4, 5,
        5, 1, 0, 
        
        //top face
        3, 6, 7,
        3, 2, 6,

        //front face
        7, 6, 4,
        6, 5, 4,

        //back face
        2, 3, 1,
        3, 0, 1,

        //left face 
        3, 7, 0,
        7, 4, 0,

        //right face 
        6, 2, 5,
        2, 1, 5
    };

    for(int i = 0; i < 36; i++)
        indices[i] = trianglesIndices[i];

    
    glCreateBuffers(1, &skybox.VBO);
    glNamedBufferStorage(skybox.VBO,vertexCount *  sizeof(vec3), vertices, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &skybox.EBO);
    glNamedBufferStorage(skybox.EBO,indexCount * sizeof(unsigned int), indices, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &skybox.VAO);
    glVertexArrayVertexBuffer(skybox.VAO, 0, skybox.VBO, 0, sizeof(vec3));
    glVertexArrayElementBuffer(skybox.VAO, skybox.EBO);

    glEnableVertexArrayAttrib(skybox.VAO, 0);
    glVertexArrayAttribFormat(skybox.VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(skybox.VAO, 0, 0);



    const char* textureNames[6] = {"../Assets/posx.png",
								"../Assets/negx.png",
								"../Assets/posy.png",
								"../Assets/negy.png",
								"../Assets/posz.png",
								"../Assets/negz.png"};



/*

    const char* textureNames[6] = {"../Assets/pposx.jpg",
								"../Assets/pnegx.jpg",
								"../Assets/pposy.jpg",
								"../Assets/pnegy.jpg",
								"../Assets/pposz.jpg",
								"../Assets/pnegz.jpg"};
*/


/*
    const char* textureNames[6] = {"../Assets/night_posx.png",
								"../Assets/night_negx.png",
								"../Assets/night_posy.png",
								"../Assets/night_negy.png",
								"../Assets/night_posz.png",
								"../Assets/night_negz.png"};
*/
    //night_posx
	int texture_widths[6];
	int texture_heights[6];
	int channels[6];
	GLubyte* Data[6];

	printf("Loading skybox images: ...\n");
	for(int i=0;i<6;i++) 
    {
		printf("\tLoading: %s ...", textureNames[i]);
		Data[i] = stbi_load(textureNames[i],	&texture_widths[i], &texture_heights[i], &channels[i], 0);
		printf("done.\n");
	}


    glCreateTextures(GL_TEXTURE_CUBE_MAP,1, &skybox.texture);
    GLint sTexture = 0;
    glBindTextureUnit(sTexture, skybox.texture);

	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);

	


	GLint format = (channels[0]==4) ? GL_RGBA : GL_RGB;
    
	for(int i=0;i<6;i++) 
    {

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, texture_widths[i], texture_heights[i], 0, format, GL_UNSIGNED_BYTE, Data[i]);
		stbi_image_free(Data[i]);
	} 
}

void DrawSkybox(Skybox& skybox, Shader* shader, Camera& camera)
{
    mat4 model = Scale(vec3(1000, 1000, 1000));
    model = model * ZRotation(180);
    //model = model * XRotation(-107) * YRotation(-23);
    mat4 modelviewProj = model * camera.viewProjection;

    glBindProgramPipeline(shader->pipeline);
    glBindVertexArray(skybox.VAO);

    glProgramUniformMatrix4fv(shader->vertex, 0, 1, GL_TRUE, modelviewProj.asArray);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindProgramPipeline(0);
    glBindVertexArray(0);
}


struct MatrixBlock
{
    mat4 projMatrix;
    mat4 viewMatrix;
};


int main(int argc, const char** argv)
{

    GetWglFunctions();

    WNDCLASSEXW wc = {};
    
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = MainWindow32Callback;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"OpenGLWindowClass";
	ATOM atom = RegisterClassExW(&wc);
	Assert(atom && "Failed to register window class");

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT windowRect;
	SetRect(&windowRect, (screenWidth / 2) - (gScreenWidth / 2),
	 (screenHeight / 2) - (gScreenHeight / 2),
	 (screenWidth / 2) + (gScreenWidth / 2),
	 (screenHeight / 2) + (gScreenHeight / 2));

    DWORD exstyle = WS_EX_APPWINDOW; //WS_EX_LAYERED;
    DWORD style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRectEx(&windowRect, style, FALSE, 0);

    HWND window = CreateWindowExW(
        exstyle, wc.lpszClassName, L"Game Window", style,
        windowRect.left, windowRect.top, windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
        NULL, NULL, wc.hInstance, NULL);

	//SetLayeredWindowAttributes(window, 0, 200, LWA_ALPHA);
    Assert(window && "Failed to create window");

    HDC dc = GetDC(window);
    Assert(dc && "Failed to get window device context");

    InitWin32OpenGL(window);

	BOOL vsync = TRUE;
	wglSwapIntervalEXT(vsync ? 1 : 0);

	ShowWindow(window, SW_SHOWDEFAULT);

	LARGE_INTEGER freq, c1;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&c1);

	Memory memory = {};
	Memory tempMemory = {};
	Input input = {};
	memory = InitMemory(8LL * GigaByte, 4LL * GigaByte);
	tempMemory = InitMemory(2LL * GigaByte, 1LL * GigaByte);


    Shader shader;
    shader.vertexPath = "../Shaders/Basic.vert",
    shader.fragmentPath =  "../Shaders/Basic.frag";

    size_t used = memory.usedSize;

    CompileShader(&memory, &shader);
    memory.usedSize = used; // restore memory


    GLsizeiptr bufferSize = sizeof(MatrixBlock);
    GLuint uboMatrice;
    glCreateBuffers(1, &uboMatrice);
    glNamedBufferStorage(uboMatrice, bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrice,0, bufferSize);



    Model conference = CreateObjVAO("../Assets/conference.obj", &memory);

    Model Ahri = CreateObjVAO("../Assets/AhriKDA.obj", &memory);

    Model Fiora = CreateObjVAO("../Assets/Fiora.obj", &memory);

    Model Alduin = CreateObjVAO("../Assets/Alduin.obj", &memory);

    Model Bunny = CreateObjVAO("../Assets/bunny.obj", &memory);

    Model Robot = CreateObjVAO("../Assets/robot.obj", &memory);

    Model Cube = CreateCube();
    Cube.meshes[0].material.diffuse = vec3(0.5f,0.0f , 0.5f);


    mat4* transforms[7];
    transforms[0] = (mat4 *)MyMalloc(&tempMemory, sizeof(mat4));
    transforms[0] = &conference.modelMatrix;
    transforms[1] = (mat4 *)MyMalloc(&tempMemory, sizeof(mat4));
    transforms[1] = &Ahri.modelMatrix;
    transforms[2] = (mat4 *)MyMalloc(&tempMemory, sizeof(mat4));
    transforms[2] = &Fiora.modelMatrix;
    transforms[3] = (mat4 *)MyMalloc(&tempMemory, sizeof(mat4));
    transforms[3] = &Alduin.modelMatrix;
    transforms[4] = (mat4 *)MyMalloc(&tempMemory, sizeof(mat4));
    transforms[4] = &Bunny.modelMatrix;
    transforms[5] = (mat4 *)MyMalloc(&tempMemory, sizeof(mat4));
    transforms[5] = &Robot.modelMatrix;
    transforms[6] = (mat4 *)MyMalloc(&tempMemory, sizeof(mat4));
    transforms[6] = &Cube.modelMatrix;

    Camera camera = {};
    InitCamera(camera);


    MatrixBlock matrixBlock = 
    {
        .projMatrix = mat4(),
        .viewMatrix = mat4()
    };

    const char* sourceDLL="Platform.dll";
    const char* tempDLL="PlatformTemp.dll";
    const char* lockFileName = "lock.tmp ";
    GameLibrary Game =  LoadGameLibrary(sourceDLL, tempDLL, lockFileName);
	Running = true;


    while(Running)
    {
		FILETIME newDLLWriteTime = GetLastWriteTime(sourceDLL);
        if(CompareFileTime(&newDLLWriteTime, &Game.lastWriteTime) != 0)
        {
            UnloadGameLibrary(&Game);
            Game =  LoadGameLibrary(sourceDLL, tempDLL,lockFileName);
        }


        RECT rect;
        GetClientRect(window, &rect);
        gScreenWidth = rect.right - rect.left;
        gScreenHeight = rect.bottom - rect.top;

        WindowDimension windowDimension;
        windowDimension.width = gScreenWidth;
        windowDimension.height = gScreenHeight;

        LARGE_INTEGER c2;
        QueryPerformanceCounter(&c2);
        float delta = (float)((double)(c2.QuadPart - c1.QuadPart) / freq.QuadPart);
        c1 = c2;
        frameCounter++;

        static float angle = 0.0f;
        angle += 50.0f * delta;

		if(input.Escape)
		{
			Running = false;
			
		}

		Game.UpdateGame(window,windowDimension,&input,&camera,delta,transforms);
        ProcessInput(input,windowDimension,&memory );
        UpdateCamera(camera, windowDimension, input,delta);


        matrixBlock.projMatrix = Transpose(camera.projection);
        matrixBlock.viewMatrix = Transpose(camera.view);
   
        glNamedBufferSubData(uboMatrice, 0, bufferSize, &matrixBlock);



        if (gScreenWidth != 0 && gScreenHeight != 0)
        {
            glViewport(0, 0, gScreenWidth, gScreenHeight);

        
            //glClearColor(0.392f, 0.584f, 0.929f, 1.f);
            glClearColor(0.1f, 0.1f, 0.1f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            DrawModel(conference, &shader, camera);


            DrawModel(Ahri, &shader,camera);

            DrawModel(Fiora, &shader,camera);

            DrawModel(Alduin, &shader,camera);

            if((frameCounter / 60) % 2)
                DrawModel(Bunny, &shader,camera);

            DrawModel(Robot, &shader,camera);

            DrawModel(Cube, &shader,camera);
            
            if (!SwapBuffers(dc))
            {
                FatalError("Failed to swap OpenGL buffers!");
            }
        }
        else
        {
            // window is minimized, cannot vsync - instead sleep a bit
            if (vsync)
            {
                Sleep(10);
            }
        }
    }

    DeInitMemory(&memory);
    printf("Memory info :\tmemory.usedSize: %zd\t  memory.tempUsedSize: %zd\n", memory.usedSize, memory.tempUsedSize);
    return 0;
}
