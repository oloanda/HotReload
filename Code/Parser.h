#pragma once
#include <stdlib.h>
#include "Vectors.h"

bool IsWhiteSpace(char c)
{
    return (c == ' ' || c == '\t');
}

char* SkipeWhiteSpace(char* str)
{
    while(!IsWhiteSpace(*str))
        str++;
    while(IsWhiteSpace(*str))
        str++;
    return str;
}

char* GetNextLine(char* str)
{
    char *result;
    if(*str == '\n')
        result = str + 1;
    else
        result = str;
    while(*result != '\n' && *result != '\0')
        result++;

    return result;
}

vec2 GetVec2(char *&str)
{
    vec2 result;
    result.x = (float)atof(str);
    str = SkipeWhiteSpace(str);
    result.y = (float)atof(str);

    return result;
}

vec3 GetVec3(char *&str)
{
    vec3 result;
    result.x = (float)atof(str);
    str = SkipeWhiteSpace(str);
    result.y = (float)atof(str);
    str = SkipeWhiteSpace(str);
    result.z = (float)atof(str);

    return result;
}