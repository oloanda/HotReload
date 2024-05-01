#pragma once
#include "Matrices.h"

struct CameraInput
{
    int mouseX;
    int mouseY;
    bool rotateCamera;
    bool moveUp;
    bool moveDown;
    bool moveForward;
    bool moveBackward;
    bool moveLeft;
    bool moveRight;
};

struct Camera
{
    CameraInput input = {};
    mat4 projection = {};
    mat4 view = {};
    mat4 viewProjection = {};


    vec3 position = {};
    vec3 rotation = {};
    vec3 acceleration = {};
    vec3 scale = {};

};

void InitCamera(Camera &camera)
{
    //camera.position = vec3(0, 0, -2);
    //camera.position = vec3(2, -54.32f, 79.0f);
    camera.position = vec3(-87.0f, -54.32f, -85.6f);
    camera.scale = vec3(1, 1, 1);
    camera.rotation.y = PiOver2;
}
