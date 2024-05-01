#pragma once
#include "Matrices.h"

struct Light
{
    vec4 position;

    vec3 ambient;
    float constant;


    vec3 diffuse;
    float linear;

    vec3 specular;
    float quadratic;

};
