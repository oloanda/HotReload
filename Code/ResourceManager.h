#pragma once
#include <memoryapi.h>
#include <stdio.h>

#include "MyString.h"

#include <intrin.h>
#define Assert(cond) do {if(!(cond)) __debugbreak();} while(0)

#define KiloByte 1024LL
#define MegaByte 1024LL * KiloByte
#define GigaByte 1024LL * MegaByte
#define TeraByte 1024LL  * GigaByte



struct Memory
{
    size_t totalSize;
    size_t usedSize;
    void* storage;

    size_t tempSize;
    size_t tempUsedSize;
    void* tempStorage;
};



inline Memory InitMemory(size_t size, size_t tempSize)
{
    Memory memory = {};
    printf("Total Memory size: %zd\n", size);
    memory.totalSize = size - tempSize;
    memory.tempSize = tempSize;
    memory.storage = VirtualAlloc(0,size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    memory.tempStorage = (void *)((size_t)memory.storage + memory.totalSize);
    memory.usedSize = 0;
    memory.tempUsedSize = 0;
    if(memory.storage == NULL)
        __debugbreak();
    return memory;
}

inline void DeInitMemory(Memory* memory)
{
    Memory result = {0,0,NULL, 0,0,NULL};
    VirtualFree(memory->storage, 0, MEM_RELEASE);
    *memory = result;
}

inline void* MyMalloc(Memory* memory, size_t size)
{
    if(memory->usedSize + size >= memory->totalSize)
        __debugbreak();
    size_t currentOffset = (size_t)memory->storage + memory->usedSize;
    memory->usedSize += size + (64 - size % 64);
    return ((void *)currentOffset);
}

inline void* MyTempAlloc(Memory* memory, size_t size)
{
    if(memory->tempUsedSize + size >= memory->tempSize)
        __debugbreak();

    size_t currentOffset = (size_t)memory->storage + memory->totalSize + memory->tempUsedSize;
    memory->tempUsedSize += size + (64 - size % 64);
    return ((void *)currentOffset);
}

inline void MyFree(Memory* memory, size_t size)
{
    size_t currentOffset = (size_t)memory->storage + memory->usedSize - size;
    memset((void *)currentOffset, 0, size);
    memory->usedSize -= size;
}

struct FileResource
{
    FILE* file;
    MyString path;
    void* data;
    size_t size;

    inline FileResource(const char* fileName, Memory* memory)
    {
        file = nullptr;
        path.size = strlen(fileName) + 255 + 1;
        path.data = (char *)MyMalloc(memory, path.size);
        strcpy_s(path.data, path.size, fileName);
        data = nullptr;
        size = 0;
    }
};

inline void LoadFile(FileResource& resource, Memory* memory, bool hash, char* buffer)
{
    if(resource.file != nullptr)
        fclose(resource.file);
    
    fopen_s(&resource.file, resource.path.data, "rb");
    if(resource.file != nullptr)
    {
        fseek(resource.file, 0, SEEK_END);
        resource.size = ftell(resource.file);
        fseek(resource.file, 0, SEEK_SET);
        resource.data = MyMalloc(memory, resource.size);
        fread(resource.data, resource.size, 1, resource.file);

    }
}