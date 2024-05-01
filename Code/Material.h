#pragma once
#include "Texture.h"
#include "Vectors.h"




struct Material
{
    vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
    vec3 diffuse = vec3(0.8f, 0.8f, 0.8f);
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);
    vec3 emissive = vec3(0.0f, 0.0f, 0.0f);
    float shininess = 25.0f;
    float opaqueness = 1.0f;
    Texture diffuseMap = {};
    Texture specularMap = {};
    Texture normalMap = {};
};