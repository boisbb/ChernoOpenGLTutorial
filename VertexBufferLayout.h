#include <vector>
#include <iostream>

#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <GL/glew.h>
#include "Renderer.h"
#include <iostream>

struct VertexBufferElement{
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int GetSizeOfType(unsigned int type){
    switch(type){
      case GL_FLOAT: return 4;
      case GL_UNSIGNED_INT: return 4;
      case GL_UNSIGNED_BYTE: return 1;
    }
    ASSERT(false);
    return 0;
  }
};


class VertexBufferLayout{
private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;
public:
  VertexBufferLayout()
    : m_Stride(0) {}

  template<typename T>
  void Push(unsigned int count){
    std::cout << "nespravnej" << std::endl;
    ASSERT(false);
  }

  inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
  inline unsigned int GetStride() const { return m_Stride; }
};

#endif
