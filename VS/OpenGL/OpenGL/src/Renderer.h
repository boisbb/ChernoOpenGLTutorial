#pragma once

#include <GL/glew.h>

#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

using namespace std;

class Renderer {
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};