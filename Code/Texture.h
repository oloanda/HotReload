#pragma once
#include "OpenGLGraphics.h"

#include <map>
#include "meow_hash_x64_aesni.h"
#include "ResourceManager.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assert.h>

class Texture
{
public:
    unsigned int index = 0;
    int width = 0;
    int height = 0;
    int nbChannels = 0;
    inline static std::map<size_t, int> textureMap;
    inline Texture() {}
    inline Texture(const char* str, Memory* memory)
    {
        char myTemp[255] = {};
        bool stbi = false;

        size_t usedSize = memory->usedSize;

        FileResource file(str, memory);
        LoadFile(file, memory, false, &myTemp[0]);

        meow_u128 Hash = MeowHash(MeowDefaultSeed, file.size, file.data);
        size_t hash64 = MeowU64From(Hash, 0);
        if(textureMap[hash64] != 0)
        {
            index = textureMap[hash64];
            memory->usedSize = usedSize;
            return;
        }
        char tempBuffer[128];
        snprintf(tempBuffer, 128,"Temp/%02zX.img", hash64);

        new (&file) FileResource(tempBuffer, memory);
        LoadFile(file, memory, true, NULL);
        unsigned char *data;
        if(file.file)
        {
            *this = *((Texture *)file.data);
            data = (unsigned char *)((size_t)file.data + sizeof(Texture));
            fclose(file.file);
        }
        else
        {
            FILE* imageFile = nullptr;

            stbi = true;
            stbi_set_flip_vertically_on_load(true);
            if(myTemp[0] == '\0')
                data = stbi_load(str, &width, &height, &nbChannels, 0);
            else
                data = stbi_load(myTemp, &width, &height, &nbChannels, 0);
            
            fopen_s(&imageFile, tempBuffer, "wb");
            if(imageFile)
            {
                fwrite((void *)this, sizeof(Texture), 1, imageFile);
                fwrite((void *)data, width * height * nbChannels, 1, imageFile);
                fclose(imageFile);
            }
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &index);
        if (nbChannels == 4)
        {
            glTextureStorage2D(index, 1, GL_RGBA8, width, height);
            glTextureSubImage2D(index, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else if (nbChannels == 3)
        {
            glTextureStorage2D(index, 1, GL_RGBA8, width, height);
            glTextureSubImage2D(index, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
            assert(0);

        glTextureParameteri(index, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(index, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(index, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(index, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTextureParameterf(index, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.f);

        glGenerateTextureMipmap(index);
        if (textureMap[hash64] == 0)
            textureMap[hash64] = index;
        if (stbi)
            stbi_image_free(data);
        memory->usedSize = usedSize;

        
    }
};



