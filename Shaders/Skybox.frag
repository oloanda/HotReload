#version 450 core


layout(location = 0) out vec4 FragColor;

layout(binding = 0)
uniform samplerCube cubeMap;


smooth in vec3 TexCoord;


void main()
{
    FragColor = texture(cubeMap, TexCoord);
}