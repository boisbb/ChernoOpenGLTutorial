#pragma once

#include <array>

struct Vec2{
  float x, y;
};

struct Vec3{
  float x, y, z;
};

struct Vec4{
  float x, y, z, w;
};


struct Vertex{
  Vec3 Position;
  Vec4 Color;
  Vec2 TexCoords;
  float TexID;
};


class VertexBuffer{
private:
  unsigned int m_RendererID;
public:
  VertexBuffer(const void* data, unsigned int count);
  VertexBuffer(unsigned int size);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;
};


