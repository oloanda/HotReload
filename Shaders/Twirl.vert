#version 450 core

layout(location = 0) in vec2 inPosition;

smooth out vec2 TexCoord;

void main()
{
    gl_Position = vec4(inPosition * 2.0f - 1, 0.0f, 1.0f);
    TexCoord = inPosition;
}