#pragma once

#include "Renderer.h"

class Texture{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    unsigned int unit;
public:
    Texture(const std::string& path, GLenum texType = GL_TEXTURE_2D, GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE);
    ~Texture();
    
    void Bind(unsigned int slot = 0);
    void Unbind() const;

    inline int GetWidth() const{ return m_Width; }
    inline int GetHeight() const{ return m_Height; }
};