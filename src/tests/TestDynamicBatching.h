#pragma once

#include "Test.h"

#include <memory>

#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"

namespace test {
    class TestDynamicBatching : public Test{
    public:
        TestDynamicBatching();
        ~TestDynamicBatching();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private: 
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_TextureA;
        std::unique_ptr<Texture> m_TextureB;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        glm::vec3 m_TranslationA, m_TranslationB;
        glm::mat4 m_Proj, m_View;
    };
}