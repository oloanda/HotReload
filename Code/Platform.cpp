#include "Platform.h"
#include <stdio.h>




static void ProcessMessage(HWND window,Input* input)
{
    MSG Message;
    while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
    {
        switch(Message.message)
        {
            case WM_QUIT:
            {
                //Running = false;
            }
            break;
            case WM_MOUSEMOVE: 
            {
                int mouseX = input->mousePositionX;
                int mouseY = input->mousePositionY;
                input->mousePositionX = GET_X_LPARAM(Message.lParam);
                input->mousePositionY = GET_Y_LPARAM(Message.lParam);
                input->mouseDeltaX = input->mousePositionX - mouseX;
                input->mouseDeltaY = input->mousePositionY - mouseY;
   
            }
            break;
            case WM_MOUSEWHEEL:
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            break;
            case WM_LBUTTONDOWN:
            {
                input->mouseLClick = true;
                SetCapture(window);
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            break;
            case WM_LBUTTONUP:
            {
                input->mouseLClick = false;
                ReleaseCapture();
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            break;
            case WM_RBUTTONDOWN: 
            {
                input->mouseRClick = true;
                SetCapture(window);
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            break;
            case WM_RBUTTONUP:
            {
                input->mouseRClick = false;
                ReleaseCapture();
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            break;
            case WM_KEYUP:
            case WM_KEYDOWN:
            {
                int VKCode = (int)Message.wParam;

                int isDown = ( (Message.lParam & (1 << 31))  == 0);
                UINT scancode = MapVirtualKeyEx(VKCode, MAPVK_VK_TO_VSC, GetKeyboardLayout(0));


                switch(scancode)
                {
                    case 0x01: input->Escape = isDown; break; // Escape
                    case 0x10: input->Q = isDown; break; // Q
                    case 0x11: input->W = isDown; break; // W
                    case 0x12: input->E = isDown; break; // E
                    case 0x13: input->R = isDown; break; // R
                    case 0x14: input->T = isDown; break; // T
                    case 0x15: input->Y = isDown; break; // Y
                    case 0x16: input->U = isDown; break; // U
                    case 0x17: input->I = isDown; break; // I
                    case 0x18: input->O = isDown; break; // O
                    case 0x19: input->P = isDown; break; // P
                    case 0x1A: input->OpenBracket = isDown; break; // [
                    case 0x1B: input->CloseBracket = isDown; break; // ]
                    case 0x1C: input->Enter = isDown; break; // Enter / Return
                    case 0x1D: input->LControl = isDown; break; // LCtrl
                    case 0x1E: input->A = isDown; break; // A
                    case 0x1F: input->S = isDown; break; // S
                    case 0x20: input->D = isDown; break; // D
                    case 0x21: input->F = isDown; break; // F
                    case 0x22: input->G = isDown; break; // G
                    case 0x23: input->H = isDown; break; // H
                    case 0x24: input->J = isDown; break; // J
                    case 0x25: input->K = isDown; break; // K
                    case 0x26: input->L = isDown; break; // L
                    case 0x27: input->SemiColon = isDown; break;// ;
                    case 0x28: input->Apostrophe = isDown; break;// '
                    case 0x29: input->Tilda = isDown; break;// `~
                    case 0x2A: input->LShift = isDown; break;// LShift
                    case 0x2B: input->BackSlash = isDown; break;// \|
                    case 0x2D: input->X = isDown; break;// X
                    case 0x2E: input->C = isDown; break;// C
                    case 0x2F: input->V = isDown; break;// V
                    case 0x30: input->B = isDown; break;// B
                    case 0x31: input->N = isDown; break;// N
                    case 0x32: input->M = isDown; break;// M
                    case 0x33: input->Comma = isDown; break;// <
                    case 0x34: input->Period = isDown; break;// >
                    case 0x35: input->Slash = isDown; break;// /?
                    case 0x36: input->RShift = isDown; break;// RShift
                    // 37 (Keypad-*) or (*/PrtScn) on a 83/84-key keyboard
                    // 38 (LAlt),
                    case 0x39: input->Spacebar = isDown; break; // Space bar
                    // 3a (CapsLock)
                    // 3b (F1), 3c (F2), 3d (F3), 3e (F4), 3f (F5), 40 (F6), 41 (F7), 42 (F8), 43 (F9), 44 (F10)
                    case 0x48: input->Up = isDown; break; // Up
                    case 0x4b: input->Left = isDown; break; // Left
                    case 0x4d: input->Right = isDown; break; // Right
                    case 0x50: input->Down = isDown; break; // Down
                    case 0x53: input->Delete = isDown; break;
                    default: { }
                }
            }
            break;
            default:
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            break;
        }
    }

}


extern "C" UPDATE_GAME(UpdateGame)
{
    static bool initMemory = false;
    if(!initMemory)
    {

        initMemory = true;
    }

    ProcessMessage(window,input);




    camera->input.moveForward = input->W;
    camera->input.moveBackward = input->S;
    camera->input.moveLeft = input->A;
    camera->input.moveRight = input->D;
    camera->input.moveUp = input->Q;
    camera->input.moveDown = input->E;
    camera->input.rotateCamera = input->mouseRClick;

    camera->input.mouseX = input->mousePositionX;
    camera->input.mouseY = input->mousePositionY;



    static float angle = 0.0f;
    angle += 50.0f * deltaTime;

    //camera.position = vec3(-95.2f, -60.31f, -84.5f);

    //*transforms[0] = 

    Printvec3(camera->position);



    //Conference
    //*transforms[0] = Scale(vec3(1, 1, 1));
    *transforms[0] = Scale(vec3(0.2f, 0.2f, 0.2f));
    *transforms[0] = *transforms[0] * YRotation(-90);
    *transforms[0] = *transforms[0] * ZRotation(-180);
    *transforms[0] = *transforms[0] * Translation(vec3(0, 0, 6));

    //Printmat4(*transforms[0]);
    
    //Ahri
    *transforms[1] = Scale(vec3(2, 2, 2));
    *transforms[1] = *transforms[1] * XRotation(180);
    *transforms[1] = *transforms[1] * YRotation(angle);
    *transforms[1] = *transforms[1] * Translation(vec3(50, 0, 300));

    //Fiora

    *transforms[2]  = Transform(vec3(0.6f, 0.6f, 0.6f), vec3(180, -45, 0), 
    vec3(-160, 0, 300));


    //Alduin
    *transforms[3] = Scale(vec3(0.04f, 0.04f, 0.04f));
    *transforms[3]  = *transforms[3] * YRotation(-90) * ZRotation(180); 
    *transforms[3]  = *transforms[3] * Translation(vec3(-40, -35, 100));

    //Bunny
    *transforms[4] = Scale(vec3(10, 10, 10));
    *transforms[4] = *transforms[4] * ZRotation(-180);
    *transforms[4] = *transforms[4] * YRotation(-angle);
    *transforms[4] = *transforms[4] * Translation(vec3(-30, -35, 50));

    //Robot
    *transforms[5] = Scale(vec3(12, 12, 12));
    *transforms[5] = *transforms[5] * ZRotation(-180);
    *transforms[5] = *transforms[5] * YRotation(-225);
    *transforms[5] = *transforms[5] * Translation(vec3(-150, 0, 100));


    //Cube
    *transforms[6] = Scale(vec3(30, 30, 30));
    //*transforms[6] = Scale(vec3(100, 100, 100));
    *transforms[6] = *transforms[6] * Translation(vec3(-110, -15, 100));

}