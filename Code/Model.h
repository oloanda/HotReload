#pragma once
#include "Texture.h"
//#include "OpenGLGraphics.h"
#include "Matrices.h"
#include "ResourceManager.h"


#include "Material.h"

#include "meow_hash_x64_aesni.h"
#include <unordered_map>
#include "Parser.h"


struct Vertex
{
    vec3 position = {};
    vec3 normal = {};
    vec2 textureUV = {};
    Vertex(){};
	Vertex(vec3 pos, vec3 n, vec2 tex)
    {position = pos; normal = n; textureUV = tex;}
    struct HashFunction
    {
        size_t operator()(const Vertex& v) const
        {
            meow_u128 Hash = MeowHash(MeowDefaultSeed, sizeof(Vertex), (void *)&v);
            long long unsigned Hash64 = MeowU64From(Hash, 0);

            return (Hash64);
        }
    };
};

inline bool operator==(const Vertex &a, const Vertex &b)
{
    if (a.position.x == b.position.x && a.position.y == b.position.y &&
        a.position.z == b.position.z && a.normal.x == b.normal.x && a.normal.y == b.normal.y &&
        a.normal.z == b.normal.z && a.textureUV.x == b.textureUV.x && a.textureUV.y == b.textureUV.y)
        return (true);
    return (false);
}



struct Mesh
{
    Vertex* vertices = nullptr;
    unsigned int* indices = nullptr;
    unsigned int vertexCount = 0;
    unsigned int indexCount = 0;
    Material  material = {};
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
};


struct Model
{
    Mesh* meshes = nullptr;
    size_t meshCount = 0;
    mat4 modelMatrix;
};


void GenerateBuffers(Mesh* meshes, size_t meshCount)
{
    for(int i = 0; i < meshCount; i++)
    {
        glCreateBuffers(1, &meshes[i].VBO);
        glNamedBufferStorage(meshes[i].VBO, sizeof(Vertex) * meshes[i].vertexCount, meshes[i].vertices, GL_DYNAMIC_STORAGE_BIT);

        glCreateBuffers(1, &meshes[i].EBO);
        glNamedBufferStorage(meshes[i].EBO, sizeof(unsigned int) * meshes[i].indexCount, meshes[i].indices, GL_DYNAMIC_STORAGE_BIT);

        glCreateVertexArrays(1, &meshes[i].VAO);

        glVertexArrayVertexBuffer(meshes[i].VAO,0, meshes[i].VBO,0, sizeof(Vertex));
        glVertexArrayElementBuffer(meshes[i].VAO, meshes[i].EBO);

        glEnableVertexArrayAttrib(meshes[i].VAO, 0);
        glEnableVertexArrayAttrib(meshes[i].VAO, 1);
        glEnableVertexArrayAttrib(meshes[i].VAO, 2);

        glVertexArrayAttribFormat(meshes[i].VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
        glVertexArrayAttribFormat(meshes[i].VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
        glVertexArrayAttribFormat(meshes[i].VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, textureUV));

        glVertexArrayAttribBinding(meshes[i].VAO, 0, 0);
        glVertexArrayAttribBinding(meshes[i].VAO, 1, 0);
        glVertexArrayAttribBinding(meshes[i].VAO, 2, 0);

    }
}


Material ParseMTL(const char* buffer,char* MaterialName ,Memory* memory)
{
    Material material = {};
    material.diffuseMap.index = 0;
    material.normalMap.index = 0;
    material.specularMap.index = 0;
    material.ambient = vec3(0.5f, 0.5f, 0.5f);
    material.diffuse = vec3(0.8f, 0.8f, 0.8f);
    material.specular = vec3(1.0f, 1.0f, 1.0f);
    material.emissive = vec3(1.0f, 1.0f, 1.0f);
    material.opaqueness = 1.0f;
    material.shininess = 25.0f;

    char* current   = (char *)buffer;
    char* endOfLine = (char *)buffer;

    char temp[255] = {};
    int i = 0;
    while(MaterialName[i] != '\n' && MaterialName[i] != ' ' && MaterialName[i] != '\r')
    {
        temp[i] = MaterialName[i];
        i++;
    }
    temp[i] = '\0';
    while(*endOfLine != '\0')
    {
        current = endOfLine;
        endOfLine = GetNextLine(endOfLine);
        while(current != endOfLine && endOfLine > current)
        {
            if(*current == '#')
                current = endOfLine;
            if(strncmp(temp, current, strlen(temp)) == 0 && (current[strlen(temp)] == '\n' || current[strlen(temp)] == '\r'))
            {
                while(strncmp(current, "newmtl ", 7) != 0 && *current != '\0')
                {
                    if(strncmp(current, "norm ", 5) == 0)
                    {
                        i = 0;
                        while(current[i] != '\n' && current[i] != ' ' && current[i] != '\t' && current[i] != '\r')
                            i++;
                        i++;
                        int j = 0;
                        while(current[i] != '\n' && current[i] != ' ' && current[i] != '\t' && current[i] != '\r')
                            temp[j++] = current[i++];
                        temp[j] = '\0';
                        material.normalMap = Texture(&temp[0], memory);
                        current += 5;
                    }
                    else if(strncmp(current, "map_Kd ", 7) == 0)
                    {
                        i = 0;
                        while(current[i] != '\n' && current[i] != ' ' && current[i] != '\t' && current[i] != '\r')
                            i++;
                        i++;
                        int j = 0;
                        while(current[i] != '\n' && current[i] != ' ' && current[i] != '\t' && current[i] != '\r')
                            temp[j++] = current[i++];
                        temp[j] = '\0';
                        material.diffuseMap = Texture(&temp[0], memory);
                        current += 7;
                    }
                    else if(strncmp(current, "map_Ks ", 7) == 0)
                    {
                        i = 0;
                        while(current[i] != '\n' && current[i] != ' ' && current[i] != '\t' && current[i] != '\r')
                            i++;
                        i++;
                        int j = 0;
                        while(current[i] != '\n' && current[i] != ' ' && current[i] != '\t' && current[i] != '\r')
                            temp[j++] = current[i++];
                        temp[j] = '\0';
                        material.specularMap = Texture(&temp[0], memory);
                        current += 7;
                    }
                    else if(strncmp(current, "Kd ", 3) == 0)
                    {
                        current = SkipeWhiteSpace(current);
                        material.diffuse = GetVec3(current);
                    }
                    else if(strncmp(current, "Ks ", 3) == 0)
                    {
                        current = SkipeWhiteSpace(current);
                        material.specular = GetVec3(current);
                    }
                    else if(strncmp(current, "Ke ", 3) == 0)
                    {
                        current = SkipeWhiteSpace(current);
                        material.emissive = GetVec3(current);
                    }
                    else if(strncmp(current, "d ", 2) == 0)
                    {
                        current = SkipeWhiteSpace(current);
                        material.opaqueness = (float)atof(current);
                    }
                    else if(strncmp(current, "Ns ", 3) == 0)
                    {
                        current = SkipeWhiteSpace(current);
                        material.shininess = (float)atof(current);
                    }
                    current++;
                }
                return material;
            }
            current++;
        }
    }
    return material;
}


Model CreateObjVAO(const char* fileName, Memory* memory)
{
    Model model = {};
    model.modelMatrix = mat4();
    Material material = {};

    FILE* objFile = nullptr;
    fopen_s(&objFile, fileName, "rb");
    int totalSize = 0;
    char* buffer = nullptr;
    if(objFile)
    {
        fseek(objFile, 0, SEEK_END); 
        totalSize = ftell(objFile);
        fseek(objFile, 0, SEEK_SET);
        buffer = (char *)MyMalloc(memory, totalSize);
        memset((void *)buffer,0 ,totalSize);
        fread((void *)buffer, totalSize, 1, objFile);
    }
    else
    {
        return model;
    }

    int indexCountPerLine = -1;
    int posCount = 0;
    int normalCount = 0;
    int textureUVCount = 0;
    int materialsCount = 0;
    int indexCount = 0;
    fclose(objFile);

    char* current = buffer;
    char* endOfLine = buffer;
    const char* mtlLib = nullptr;
    bool hasMtlFile = false;
    while(*endOfLine != '\0')
    {
        current = endOfLine;
        endOfLine = GetNextLine(endOfLine);

        while(current != endOfLine && endOfLine > current)
        {
            if(*current == '#')
                current = endOfLine;
            else if(*current == 'v' && current[1] == ' ')
            {
                current = SkipeWhiteSpace(current);
                posCount++;
                current = endOfLine;
            }
            else if(*current == 'v' && current[1] == 't')
            {
                current = SkipeWhiteSpace(current);
                textureUVCount++;
                current = endOfLine;
            }
            else if(*current == 'v' && current[1] == 'n')
            {
                current = SkipeWhiteSpace(current);
                normalCount++;
                current = endOfLine;
            }
            else if(strncmp("mtllib", current, 6) == 0)
            {
                current = SkipeWhiteSpace(current);
                mtlLib = current;
                current = endOfLine;
                hasMtlFile = true;
            }
            else if(( (size_t)endOfLine - (size_t)current) >= 5 && strncmp("usemtl", current, 6) == 0)
            {
                materialsCount++;
                current = endOfLine;
            }
            else if(*current == 'f' && current[1] == ' ')
            {
                if(indexCountPerLine == -1)
                {
                    indexCountPerLine = 0;
                    for(int i = 0; current[i] != '\n'; i++)
                    {
                        if(current[i] == '/')
                            indexCountPerLine++;
                    }
                }
                indexCount += indexCountPerLine / 2;
                current = endOfLine;
                if(indexCountPerLine == 0)
                    indexCount += 3;
            }
            else
                current++;
        }
    }


    int verterIndex = 0;
    int positionIndex = 0;
    int normalIndex = 0;
    int textureUVIndex = 0;
    int meshIndex = 0;
    if(!hasMtlFile)
        materialsCount = 1;
    
    Mesh* meshes = (Mesh *)MyMalloc(memory, materialsCount * sizeof(Mesh));
    vec3 *position = (vec3 *)MyTempAlloc(memory, posCount * sizeof(vec3));
    vec3 *normal = (vec3 *)MyTempAlloc(memory, normalCount * sizeof(vec3));
    vec2 *textureUV = (vec2 *)MyTempAlloc(memory, textureUVCount * sizeof(vec2));
    Vertex *vertices = (Vertex *)MyTempAlloc(memory, indexCount * sizeof(Vertex));
    unsigned int *indices = (unsigned int *)MyTempAlloc(memory, indexCount * sizeof(unsigned int *));
    char mtlLibFilePath[255] = {};


    meshes[0].material = Material();
    strncpy_s(mtlLibFilePath, "../Assets/",10);

    int i = 0;
    while(mtlLibFilePath[i] != '\0')
        i++;
    int j = 0;
    if(hasMtlFile)
    {
        while(mtlLib[j] != '\n' && mtlLib[j] != '\t' && mtlLib[j] != '\r')
        {
            mtlLibFilePath[i] = mtlLib[j++];
            i++;
        }
        mtlLibFilePath[i] = '\0';
    }

    FileResource mtlLibData(mtlLibFilePath, memory);
    LoadFile(mtlLibData, memory, false, nullptr);
    if(mtlLibData.file)
        fclose(mtlLibData.file);
    
    model.meshes = meshes;
    model.meshes[meshIndex].vertices = vertices;
    model.meshes[meshIndex].indices = indices;

    current = buffer;
    endOfLine = buffer;
    model.meshCount = materialsCount;

    while(*endOfLine != '\0')
    {
        current = endOfLine;
        endOfLine = GetNextLine(endOfLine);
        while(current != endOfLine && endOfLine > current)
        {
            if(*current == '#')
            {
                current = endOfLine;
            }
            else if(*current == 'v' && current[1] == ' ')
            {
                current = SkipeWhiteSpace(current);
                position[positionIndex] = GetVec3(current);
                positionIndex++;
                current = endOfLine;
            }
            else if(*current == 'v' && current[1] == 't')
            {
                current = SkipeWhiteSpace(current);
                textureUV[textureUVIndex] = GetVec2(current);
                textureUVIndex++;
                current = endOfLine;
            }
            else if(*current == 'v' && current[1] == 'n')
            {
                current = SkipeWhiteSpace(current);
                normal[normalIndex] = GetVec3(current);
                normalIndex++;
                current = endOfLine;
            }
            else if(strncmp("mtllib", current, 6) == 0)
            {
                current = endOfLine;
            }
            else if(( (size_t)endOfLine - (size_t)current) >= 5 && strncmp("usemtl", current, 6) == 0)
            {
                model.meshes[meshIndex].indices = &indices[verterIndex];
                model.meshes[meshIndex].vertices = &vertices[verterIndex];
                if(meshIndex >= 1)
                {
                    model.meshes[meshIndex - 1].vertexCount = (unsigned int)(&indices[verterIndex] - model.meshes[meshIndex - 1].indices);
                    model.meshes[meshIndex - 1].indexCount = (unsigned int)(&indices[verterIndex] - model.meshes[meshIndex - 1].indices);
                }
                model.meshes[meshIndex].material = ParseMTL((char *)mtlLibData.data, &current[7], memory);
                meshIndex++;
                current = endOfLine;
            }
            else if(*current == 'f' && current[1] == ' ')
            {
                for(int j = 0; j < indexCountPerLine; current++)
                {
                    if(*current == ' ')
                    {
                        int tempIndex = atoi(&current[1]);
                        if(tempIndex < 0)
                            tempIndex = posCount + tempIndex + 1;
                        vertices[verterIndex].position = position[tempIndex - 1];
                    }
                    else if(*current == '/' && (indexCountPerLine == 8 || indexCountPerLine == 4))
                        return Model();
                    else if(*current == '/' && indexCountPerLine == 6)
                    {
                        int tempIndex = atoi(&current[1]);
                        if(j % 2 == 0)
                        {
                            if(tempIndex < 0)
                                tempIndex = textureUVCount + tempIndex + 1;
                            vertices[verterIndex].textureUV = textureUV[tempIndex - 1];
                        }
                        if(j % 2 == 1)
                        {
                            if(tempIndex < 0)
                                tempIndex = normalCount + tempIndex + 1;
                            vertices[verterIndex++].normal = normal[tempIndex - 1];
                        }
                        j++;
                    }
                }
                if(indexCountPerLine == 0)
                {
                    current = SkipeWhiteSpace(current);
                    vertices[verterIndex++].position = position[atoi(current) - 1];
                    current = SkipeWhiteSpace(current);
                    vertices[verterIndex++].position = position[atoi(current) - 1];
                    current = SkipeWhiteSpace(current);
                    vertices[verterIndex++].position = position[atoi(current) - 1];
                    vertices[verterIndex - 1].normal = GetNormalFromTriangle(vertices[verterIndex - 3].position,
                    vertices[verterIndex - 2].position, vertices[verterIndex - 1].position);
                }
            }
            else
                current++;          
        }
    }
    if(materialsCount > 1)
    {
        model.meshes[meshIndex - 1].vertexCount = (unsigned int)(&indices[verterIndex] - model.meshes[meshIndex - 1].indices);
        model.meshes[meshIndex - 1].indexCount = (unsigned int)(&indices[verterIndex] - model.meshes[meshIndex - 1].indices);     
    }
    else
    {
        model.meshes[0].vertexCount = indexCount;
        model.meshes[0].indexCount = indexCount;
    }

    for (int i = 0; i < model.meshCount; i++)
    {
        //Vertex Deduplication using a hashmap
        std::unordered_map <Vertex, int, Vertex::HashFunction> vertexMap;
        int removeSize = 0;
        for (int j = 0; j < (int)model.meshes[i].indexCount; j++)
        {
            if (vertexMap[model.meshes[i].vertices[j]] == 0)
                vertexMap[model.meshes[i].vertices[j]] = j - removeSize;
            else
                removeSize++;
            model.meshes[i].indices[j] = vertexMap[model.meshes[i].vertices[j]];
        }
        for (auto& it: vertexMap) {
            model.meshes[i].vertices[it.second] = it.first;
        }
        model.meshes[i].vertexCount = (unsigned int)vertexMap.size() + 1;

        //And generating the buffers after the deduplication
        GenerateBuffers(&model.meshes[i], 1);
    }

    memory->tempUsedSize = 0;

    return model;
}



Model CreateSphere(Memory* memory)
{
    Model model = {};
    model.meshCount = 1;

    const int stacks = 16;
    const int slices = 24;

    model.meshes = (Mesh *)MyMalloc(memory, model.meshCount * sizeof(Mesh));
    model.meshes->vertexCount = slices * (stacks - 1) + 2;
    model.meshes->indexCount = 6 * slices * (stacks - 1);
    model.meshes->vertices = (Vertex *)MyMalloc(memory, model.meshes->vertexCount * sizeof(Vertex));
    model.meshes->indices = (unsigned int *)MyMalloc(memory, model.meshes->indexCount * sizeof(unsigned int));

    int currVertex = 0;
    int currIndex = 0;

    model.meshes->vertices[currVertex].position = vec3(0.0f, -1.0f, 0.0f);
    model.meshes->vertices[currVertex].normal = vec3(0.0f, -1.0f, 0.0f);
    currVertex++;

    const float increment = Pi / (float)stacks;
    const float thetaIncrement = TwoPi / (float)slices;

    for(int latitude = 1; latitude < stacks; latitude++)
    {
        for(int longitude = 0; longitude < slices; longitude++)
        {
            float phi = -PiOver2 + float(latitude) * increment;
            float theta = float(longitude) * thetaIncrement;

            float sintheta, cosTheta;
            SinCos(theta, sintheta, cosTheta);

            float sinPhi, cosPhi;
            SinCos(phi, sinPhi, cosPhi);

            model.meshes->vertices[currVertex].position = vec3(cosTheta * cosPhi, sinPhi,sintheta * cosPhi);
            model.meshes->vertices[currVertex].normal = vec3(cosTheta * cosPhi,sinPhi,sintheta * cosPhi);

            model.meshes->vertices[currVertex].textureUV.u = (float)longitude / slices;             
            model.meshes->vertices[currVertex].textureUV.v = (float)latitude / stacks;              

            if(longitude > 0)
            {
                if(currVertex >= 1 + slices)
                {
                    model.meshes->indices[currIndex++] = currVertex + 0;
                    model.meshes->indices[currIndex++] = currVertex - 1;
                    model.meshes->indices[currIndex++] = currVertex - slices - 1;

                    model.meshes->indices[currIndex++] = currVertex + 0;
                    model.meshes->indices[currIndex++] = currVertex - slices - 1;
                    model.meshes->indices[currIndex++] = currVertex - slices;
                }
                else
                {
                    model.meshes->indices[currIndex++] = 0;
                    model.meshes->indices[currIndex++] = currVertex;
                    model.meshes->indices[currIndex++] = currVertex - 1;
                }
            }
            currVertex++;
        }

        if(currVertex >= 2 * slices)
        {
            model.meshes->indices[currIndex++] = currVertex - slices;
            model.meshes->indices[currIndex++] = currVertex - 1;
            model.meshes->indices[currIndex++] = currVertex - slices - 1;
            model.meshes->indices[currIndex++] = currVertex - slices;
            model.meshes->indices[currIndex++] = currVertex - slices - 1;
            model.meshes->indices[currIndex++] = currVertex - 2 * slices;

        }
        else
        {
            model.meshes->indices[currIndex++] = 0;
            model.meshes->indices[currIndex++] = currVertex - slices;
            model.meshes->indices[currIndex++] = currVertex - 1;    
        }
    }

    model.meshes->vertices[currVertex].position = vec3(0.0f, 1.0f, 0.0f);
    model.meshes->vertices[currVertex].normal = vec3(0.0f, 1.0f, 0.0f);

    for(int longitude = 0; longitude < slices - 1; longitude++)
    {
        model.meshes->indices[currIndex++] = currVertex;
        model.meshes->indices[currIndex++] = currVertex - longitude - 2;
        model.meshes->indices[currIndex++] = currVertex - longitude - 1;
    }
    model.meshes->indices[currIndex++] = currVertex;
    model.meshes->indices[currIndex++] = currVertex - 1;
    model.meshes->indices[currIndex++] = currVertex - slices;

    GenerateBuffers(model.meshes, 1);

    return model;
}


Model CreateCube()
{
    Model result = {};

    Mesh *tmp = new Mesh();

    result.meshes = tmp;
    result.meshCount = 1;

    Vertex vertices[] = 
    {
        //       pos                  normal                 uv
        { vec3{0.5f,-0.5f, 0.5f},  vec3{0.0f, 0.0f, 1.0f},  vec2{1.0f, 0.0f}},
        { vec3{-0.5f, 0.5f, 0.5f}, vec3{0.0f, 0.0f, 1.0f}, vec2{0.0f, 1.0f}},
        { vec3{-0.5f,-0.5f, 0.5f}, vec3{0.0f, 0.0f, 1.0f}, vec2{0.0f, 0.0f}},
        { vec3{0.5f, 0.5f, 0.5f},  vec3{0.0f, 0.0f, 1.0f},  vec2{1.0f, 1.0f}},

        {vec3{0.5,  0.5f, -0.5f},  vec3{0.0f, 0.0f, -1.0f},  vec2{1.0f, 1.0f}},
        {vec3{-0.5,-0.5f, -0.5f},  vec3{0.0f, 0.0f, -1.0f},  vec2{0.0f, 0.0f}},
        {vec3{-0.5, 0.5f, -0.5f},  vec3{0.0f, 0.0f, -1.0f},  vec2{0.0f, 1.0f}}, 
        {vec3{0.5f,-0.5f, -0.5f},  vec3{0.0f, 0.0f, -1.0f},  vec2{1.0f, 0.0f}}
    };
    tmp->vertices = new Vertex[8];
    for (int i = 0; i < 8; i++)
    {
        tmp->vertices[i] = vertices[i];
    }
    tmp->vertexCount = 8;
    int triangleIndices[36] = 
    {
        0, 1, 2,//Front
        1, 0, 3,
        4, 5, 6,//Back
        5, 4, 7,
        4, 1, 3,//Top
        1, 4, 6,
        5, 0, 2,//Bottom
        0, 5, 7,
        7, 3, 0,//Left
        3, 7, 4,
        6, 2, 1,//Right
        2, 6 ,5
    };
    tmp->indices = new unsigned int[36];
    for (int i = 0; i < 36; i++)
        tmp->indices[i] = triangleIndices[i];

    tmp->indexCount = 36;
    GenerateBuffers(tmp, 1);
    return (result);
}

Model CreateGrid(Memory* memory,int width = 1000, int depth = 1000)
{
    Model model = {};
    model.meshCount = 1;

    model.meshes = (Mesh *)MyMalloc(memory, model.meshCount * sizeof(Mesh));
    model.meshes->vertexCount = ((width + 1) + (depth + 1)) * 2;
    model.meshes->indexCount = width * depth;
    model.meshes->vertices = (Vertex *)MyMalloc(memory, model.meshes->vertexCount * sizeof(Vertex));
    model.meshes->indices = (unsigned int *)MyMalloc(memory, model.meshes->indexCount * sizeof(unsigned int));
    model.meshes[0].material = Material();
    
    int currentVertex = 0;
    int halfWidth = width / 2;
    int halfDepth = depth / 2;
    int i = 0;
    for(int i = -halfWidth; i <= halfWidth; i++)
    {
       model.meshes->vertices[currentVertex++].position = vec3(i, 0, -halfDepth); 
       model.meshes->vertices[currentVertex++].position = vec3(i, 0, halfDepth);

        model.meshes->vertices[currentVertex++].position = vec3(-halfWidth, 0, i); 
       model.meshes->vertices[currentVertex++].position = vec3(halfWidth, 0, i);  
    }

    for(int j = 0; j < width * depth - 4; j+=4)
    {
        model.meshes->indices[j] = j;
        model.meshes->indices[j + 1] = j + 1;
        model.meshes->indices[j + 2] = j + 2;
        model.meshes->indices[j + 3] = j + 3;
    }

    GenerateBuffers(model.meshes, 1);
    
    return model;

}


struct Skybox
{
    unsigned int texture = 0;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
};

struct TwirlFilter
{
    vec2 vertices[4];
    unsigned int indices[6];
    int width;
    int height;
    int nbChannels;
    unsigned int textureIndex = 0;
    //Texture texture = {};
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
};