#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
{
    Width = 0;
    Height = 0;
    InternalFormat = GL_RGB;
    ImageFormat = GL_RGB;
    WrapS = GL_REPEAT;
    WrapT = GL_REPEAT;
    FilterMin = GL_NEAREST;
    FilterMag = GL_NEAREST;

    glGenTextures(1, &ID);
}

void Texture::LoadTexture(const char *path)
{
    int width, height, channels;
    unsigned char* data = stbi_load(
        path,
        &width,
        &height,
        &channels,
        STBI_rgb_alpha
    );

    Width = width;
    Height = height;

    // Check for alpha channel
    if (channels == 4)
    {
        InternalFormat = GL_RGBA;
        ImageFormat = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        InternalFormat,
        Width,
        Height,
        0,
        ImageFormat,
        GL_UNSIGNED_BYTE,
        data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterMag);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);

    // Unload texture
    stbi_image_free(data);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, ID);
}
