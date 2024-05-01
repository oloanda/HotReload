#version 460 core

out gl_PerVertex { vec4 gl_Position; }; // required because of ARB_separate_shader_objects

layout(location  = 0) in vec3 inPosition;

layout(location = 0)
uniform mat4 modelViewProj;

smooth out vec3 TexCoord;


void main()
{
    gl_Position = vec4(inPosition, 1.0f) * modelViewProj;

    TexCoord = inPosition;
}