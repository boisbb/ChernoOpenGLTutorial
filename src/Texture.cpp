#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path, unsigned int slot, GLenum texType, GLenum format, GLenum pixelType) 
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0),
    m_Height(0), m_BPP(0), unit(slot)
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glActiveTexture(GL_TEXTURE0 + unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GLCall(glTexImage2D(texType, 0, GL_RGBA, m_Width, m_Height, 0, format, pixelType, m_LocalBuffer));
	GLCall(glGenerateMipmap(texType));

    if (m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
    }

    Unbind();
    
}


Texture::Texture(const std::string& path, unsigned int slot, const char* texType, GLenum format, GLenum pixelType) 
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0),
    m_Height(0), m_BPP(0), m_Type(texType), unit(slot)
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glActiveTexture(GL_TEXTURE0 + unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, format, pixelType, m_LocalBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));


    if (m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
    }

    std::cout << texType << std::endl;

    Unbind();
    
}

Texture::Texture(const std::string& path, unsigned int slot, const char* texType) 
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glActiveTexture(GL_TEXTURE0 + unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    if (m_BPP == 4){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
    }
    else if (m_BPP == 3){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
    }
    else if (m_BPP == 1){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, m_LocalBuffer);
    }
    else throw std::invalid_argument("Automatic Texture type recognition failed");

    //GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, format, pixelType, m_LocalBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));


    if (m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
    }

    std::cout << texType << std::endl;

    Unbind();
}

Texture::~Texture() 
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::SetUnit(Shader& shader, std::string uniform, unsigned int unit) 
{
    shader.Bind();
    shader.SetUniform1i(uniform, unit);
}

void Texture::Bind()
{
    GLCall(glActiveTexture(GL_TEXTURE0 + unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
