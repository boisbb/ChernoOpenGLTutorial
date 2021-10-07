#pragma once

#include "Test.h"

#include <memory>

#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"
#include "../Camera.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace test {
    class TestPyramid : public Test{
    public:
        TestPyramid() {};
        TestPyramid(GLFWwindow* window);
        ~TestPyramid();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetWindow(GLFWwindow* window);
    private: 
        GLFWwindow* m_Window;

        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexArray> m_LightVAO;

        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<IndexBuffer> m_LightIndexBuffer;

        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Shader> m_LightShader;

        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<VertexBuffer> m_LightVertexBuffer;

        std::unique_ptr<Camera> m_Camera;
        std::unique_ptr<Texture> m_Texture;

        float t_Rotation = 0.0f;
        std::array<Vertex, 4> CreateQuad(float x, float y, float textureID);
    };
}