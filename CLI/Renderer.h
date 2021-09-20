#define ASSERT(x) if (!(x)) exit(1);
#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#ifndef RENDERER_H_
#define RENDERER_H_

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <GL/glew.h>
#include <iostream>


using namespace std;

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer{
public:
  void Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};

#endif
