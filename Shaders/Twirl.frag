#version 450 core



out vec4 FragColor;

smooth in vec2 TexCoord;


layout(binding = 0)
uniform sampler2D textureMap;
//uniform sampler2D textureMap2;

layout(location = 0)
uniform float twirlAmount;

//3x3 sharpening kernel
const float kernel[] = float[9](-1, -1, -1,
                                 -1, 8, -1,
                                 -1, -1, -1);

void main()
{
   vec2 delta = 1.0 / textureSize(textureMap, 0);
   vec4 color = vec4(0);
   int index = 8;

   vec2 uv = TexCoord - 0.5;

   float angle = atan(uv.y, uv.x);

   //float radius = 2 * length(uv);
   float radius = length(uv);


   angle += radius * twirlAmount;

   vec2 shifted = radius * vec2(cos(angle), sin(angle));

   for(int j = -1; j <= 1; j++)
   {
      for(int i = -1; i <= 1; i++)
      {
            color += kernel[index--] * texture(textureMap, (shifted + 0.5) + (vec2(i, j) * delta));
      }
   }
   color /= 9.0;
   FragColor = color + texture(textureMap, (shifted + 0.5));
}