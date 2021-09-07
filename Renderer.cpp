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
