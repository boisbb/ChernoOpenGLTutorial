#pragma once

#include <array>
#include <vector>

#include "vendor/glm/glm.hpp"


struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;
  glm::vec2 texUV;
};

class VertexBuffer{
private:
  unsigned int m_RendererID;
public:
  VertexBuffer(const void* data, unsigned int count);
  VertexBuffer(std::vector<Vertex>& vertices);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;
};


