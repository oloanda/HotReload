#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <stddef.h>
#include <stdint.h>

#include "Matrices.h"
#include "Camera.h"

#define STR2(x) #x
#define STR(x) STR2(x)

#ifndef  M_PI
	#define M_PI 3.14159265358979323846f
#endif

typedef int8_t Int8;
typedef uint8_t UInt8;

typedef int16_t Int16;
typedef uint16_t UInt16;

typedef int32_t Int32;
typedef uint32_t UInt32;

typedef int64_t Int64;
typedef uint64_t UInt64;

typedef float Float32;
typedef double Float64;





struct WindowDimension
{
    int width;
    int height;
};


struct Input
{
    bool lockMouse = false;
    int mousePositionX;
    int mousePositionY;
    int mouseDeltaX;
    int mouseDeltaY;
    UInt8 mouseLClick : 1;
    UInt8 mouseRClick : 1;

    UInt8 Q : 1;
    UInt8 W : 1;
    UInt8 E : 1;
    UInt8 R : 1;
    UInt8 T : 1;
    UInt8 Y : 1;
    UInt8 U : 1;
    UInt8 I : 1;
    UInt8 O : 1;
    UInt8 P : 1;
    UInt8 OpenBracket : 1;
    UInt8 CloseBracket : 1;
    UInt8 BackSlash : 1;
    UInt8 A : 1;
    UInt8 S : 1;
    UInt8 D : 1;
    UInt8 F : 1;
    UInt8 G : 1;
    UInt8 H : 1;
    UInt8 J : 1;
    UInt8 K : 1;
    UInt8 L : 1;
    UInt8 SemiColon : 1;
    UInt8 Apostrophe : 1;
    UInt8 Tilda : 1;
    UInt8 LShift : 1;
    UInt8 Enter: 1;
    UInt8 LControl: 1;
    UInt8 Escape: 1;
    UInt8 Z : 1;
    UInt8 X : 1;
    UInt8 C : 1;
    UInt8 V : 1;
    UInt8 B : 1;
    UInt8 N : 1;
    UInt8 M : 1;
    UInt8 Comma : 1;
    UInt8 Period : 1;
    UInt8 Slash : 1;
    UInt8 RShift : 1;
    UInt8 Spacebar : 1;
    UInt8 Up : 1;
    UInt8 Down : 1;
    UInt8 Left : 1;
    UInt8 Right : 1;
    UInt8 Delete : 1;
};



//#define UPDATE_GAME(name) void name(HWND window,WindowDimension windowDimension,Input* input,Camera* camera,float deltaTime, mat4* transforms)
#define UPDATE_GAME(name) void name(HWND window,WindowDimension windowDimension,Input* input,Camera* camera,float deltaTime, mat4** transforms)
typedef UPDATE_GAME(update_game);
UPDATE_GAME(UpdateGameStub)
{

}

//NOTE(o.luanda): Load game dynamically
struct GameLibrary
{
    HMODULE gameDLLCode;
    FILETIME lastWriteTime;
    update_game* UpdateGame;
    bool isValid;
};

inline FILETIME GetLastWriteTime(const char* fileName)
{
    FILETIME lastWriteTime = {};
    WIN32_FIND_DATA Data;
    if(GetFileAttributesEx(fileName, GetFileExInfoStandard, &Data))
    {
        lastWriteTime = Data.ftLastWriteTime;
    }

    return lastWriteTime;
}


inline GameLibrary LoadGameLibrary(const char* sourceDLL, const char* tempDLL, const char* lockFileName)
{
    GameLibrary library = {};
    WIN32_FIND_DATA Ignored;
    if(!GetFileAttributesEx(lockFileName,GetFileExInfoStandard, &Ignored))
    {
        library.lastWriteTime = GetLastWriteTime(sourceDLL);

        CopyFileA(sourceDLL, tempDLL, FALSE);
        library.gameDLLCode = LoadLibraryA(tempDLL);
        if(library.gameDLLCode)
        {
            library.UpdateGame = (update_game*)GetProcAddress(library.gameDLLCode, "UpdateGame");
            library.isValid =  (library.UpdateGame != 0);
        }

    }

    if(!library.isValid )
    {
        library.UpdateGame = UpdateGameStub;
    }
    return library;
}


inline void UnloadGameLibrary(GameLibrary* library)
{
    if(library->gameDLLCode)
    {
        FreeLibrary(library->gameDLLCode);
        library->gameDLLCode = 0;
    }

    library->UpdateGame = UpdateGameStub;
    library->isValid = false;
}

