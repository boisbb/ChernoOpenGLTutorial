#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
http://docs.gl/
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource{
  string VertexSource;
  string FragmentSource;
};

static ShaderProgramSource ParseShader(const string filepath){

  enum class ShaderType{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };

  ifstream stream(filepath);
  string line;
  stringstream ss[2];
  ShaderType type = ShaderType::NONE;



  while (getline(stream, line)) {
    if (line.find("#shader") != string::npos ) {
      if (line.find("vertex") != string::npos) {
        type = ShaderType::VERTEX;
      }
      else if (line.find("fragment") != string::npos) {
        type = ShaderType::FRAGMENT;
      }
    }
    else{
      ss[(int)type] << line << endl;
    }
  }

  return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const string& source){
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, NULL);
  glCompileShader(id);

  int res;
  glGetShaderiv(id, GL_COMPILE_STATUS, &res);
  if (res == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    cout << "Failed to compile shader!" << endl;
    cout << message << endl;
    glDeleteShader(id);
    return 0;
  }

  // TODO: error handling

  return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader){
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    cout << "Error glewInit()" << endl;
    return 1;
  }

  cout << glGetString(GL_VERSION) << endl;
  {
    float positions[] = {
      -0.5f, -0.5f,
      0.5f, -0.5f,
      0.5f, 0.5f,

      -0.5f, 0.5f
    };

    // Index buffer
    unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);

    ShaderProgramSource src = ParseShader("res/shaders/basic.shader");

    unsigned int shader = CreateShader(src.VertexSource, src.FragmentSource);
    GLCall(glUseProgram(shader));

    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    GLCall(glUseProgram(0));
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      GLCall(glUseProgram(shader));
      GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

      va.Bind();
      ib.Bind();

      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
      // glDrawElements(GL_TRIANGLES, 3, )

      if (r > 1.0f) {
        increment = -0.05f;
      }
      else if (r < 0.0f) {
        increment = 0.05f;
      }

      r += increment;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

    glDeleteProgram(shader);

  }

  glfwTerminate();
  return 0;
}
