#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) if (!(x)) exit(1);
#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

using namespace std;

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
