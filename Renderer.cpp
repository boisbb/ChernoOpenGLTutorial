#include "Renderer.h"

void GLClearError(){
  while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line){
  while (GLenum error = glGetError()) {
    cout << "[OpenGL Error] (" << error << ")" << " from " << function << " in " << file << ":" << line << endl;
    return false;
  }
  return true;
}

void Renderer::Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{
  shader.Bind();
  va.Bind();
  ib.Bind();

  GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, NULL));
}
