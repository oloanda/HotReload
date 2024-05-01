#version 460 core
#extension GL_ARB_bindless_texture : enable


in VS_OUT
{
    vec3 FragPos;
    vec3 FragNormal;
    vec2 TexCoord;
} fs_in;

layout(location = 0) out vec4 FragColor;

layout(location = 0)
uniform bool isColor;

layout(location = 1)
uniform vec3 diffuseColor;


layout(binding = 0)
uniform sampler2D diffuseMap;


//3x3 sharpening kernel
const float kernel[] = float[9](-1, -1, -1,
                                 -1, 8, -1,
                                 -1, -1, -1);


const vec3 viewPos = vec3(0,0,0);

vec3 CalcPointLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color);

void main()
{


    vec2 delta = 1.0 / textureSize(diffuseMap, 0);
    vec4 color = vec4(0);
    int index = 8;


    if(!isColor)
    {
        for(int j = -1; j <= 1; j++)
        {
            for(int i = -1; i <= 1; i++)
            {
                color += kernel[index--] * texture(diffuseMap, fs_in.TexCoord + (vec2(i, j) * delta));
            }
        }
        color /= 9.0;
        FragColor = color + texture(diffuseMap, fs_in.TexCoord);
        //FragColor = texture(diffuseMap, fs_in.TexCoord);
    }
    else
    {
        FragColor = vec4(diffuseColor, 1);
    }

}

