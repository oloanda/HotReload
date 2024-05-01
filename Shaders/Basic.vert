#version 460 core

out gl_PerVertex { vec4 gl_Position; }; // required because of ARB_separate_shader_objects

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexture;

layout(std140, binding = 0) uniform MatrixBlock
{
    mat4 projMatrix;
    mat4 viewMatrix;
};


layout(location = 0)
uniform mat4 modelMatrix;

out VS_OUT
{
    vec3 FragPos;
    vec3 FragNormal;
    vec2 TexCoord;
} vs_out;


void main()
{
    vec4 pos = vec4(inPosition, 1.0) * modelMatrix * viewMatrix;
    vs_out.FragPos = pos.xyz;
    vs_out.FragNormal = inNormal * mat3(transpose(inverse(modelMatrix * viewMatrix)));

    vs_out.TexCoord = inTexture;
    gl_Position = pos * projMatrix;
}