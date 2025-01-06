#pragma once

#include <glad/glad.h>

class Texture
{
public:
    Texture();

    unsigned int ID;
    int Width;
    int Height;
    int InternalFormat;
    int ImageFormat;
    int WrapS;
    int WrapT;
    int FilterMin;
    int FilterMag;

    void LoadTexture(const char* path);
    void Bind() const;
};
