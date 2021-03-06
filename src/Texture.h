#pragma once

#include "Renderer.h"

class Texture{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    unsigned int unit;
    const char* m_Type;
public:
    Texture() {};
    Texture(const std::string& path, unsigned int slot = 0, GLenum texType = GL_TEXTURE_2D, GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE);
    Texture(const std::string& path, unsigned int slot, const char* texType, GLenum format, GLenum pixelType);
    Texture(const std::string& path, unsigned int slot, const char* texType);
    
    ~Texture();
    
    void SetUnit(Shader& shader, std::string uniform, unsigned int unit);
    void Bind();
    void Unbind() const;

    inline int GetWidth() const{ return m_Width; }
    inline int GetHeight() const{ return m_Height; }
    inline const char* GetType() const{ return m_Type; }
};