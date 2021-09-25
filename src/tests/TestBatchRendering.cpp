#include "TestBatchRendering.h"
#include <GL/glew.h>

#include "../Debug.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/imgui/imgui.h"
namespace test
{
    TestBatchRendering::TestBatchRendering() 
        : m_TranslationA(480, 270, 0), m_TranslationB(400, 200, 0),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {
        float positions[] = {
          -100.0f, 0.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,
          0.0f, 0.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,
          0.0f, 100.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,
          -100.0f, 100.0, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,

          0.0f, -100.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f,
          100.0f, -100.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f,
          100.0f, 0.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f,
          0.0f, 0.0, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f

        };
        unsigned int indices[] = {
          0, 1, 2,
          2, 3, 0,

          4, 5, 6,
          6, 7, 4
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 7 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(4);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/batch.shader");
        m_Shader->Bind();
        //m_Shader->SetUniform1i("u_Texture", 0);
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
    }
    
    TestBatchRendering::~TestBatchRendering() 
    {
    }
    
    void TestBatchRendering::OnUpdate(float deltaTime) 
    {
        
    }
    
    void TestBatchRendering::OnRender() 
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
    }
    
    void TestBatchRendering::OnImGuiRender() 
    {
        {   
            ImGui::SliderFloat3("Trans A", &m_TranslationA.x, 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
    }
}