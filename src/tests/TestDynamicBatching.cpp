#include "TestDynamicBatching.h"
#include <GL/glew.h>

#include "../Debug.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/imgui/imgui.h"


namespace test
{
    TestDynamicBatching::TestDynamicBatching() 
        : m_TranslationA(480, 270, 0), m_TranslationB(400, 200, 0),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {

        unsigned int indices[] = {
          0, 1, 2,
          2, 3, 0,

          4, 5, 6,
          6, 7, 4
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        //m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 10 * sizeof(float));
        m_VertexBuffer = std::make_unique<VertexBuffer>(1000);
        VertexBufferLayout layout;

        // This is very unclean due to random changes in Chernos API at the end of the series - I'm keeping it this way for now
        layout.Push<float>(3);
        layout.Push<float>(4);
        layout.Push<float>(2);
        layout.Push<float>(1);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/batch_texture.shader");
        m_Shader->Bind();
        m_TextureA = std::make_unique<Texture>("res/textures/cherno.png");
        m_TextureB = std::make_unique<Texture>("res/textures/phone.png");
        //m_Shader->SetUniform1i("u_Texture", 0);
        int samplers[2] = { 0, 1 };
        m_Shader->SetUniformMat1i("u_Textures", samplers);
    }
    
    TestDynamicBatching::~TestDynamicBatching() 
    {
    }
    
    void TestDynamicBatching::OnUpdate(float deltaTime) 
    {
        
    }
    
    void TestDynamicBatching::OnRender() 
    {
        // Set dynamic buffer
        auto q0 = CreateQuad(m_Quad1Position[0], m_Quad1Position[1], 0.0f);
        auto q1 = CreateQuad(m_Quad2Position[0], m_Quad2Position[1], 1.0f);

        Vertex vertices[8];
        memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
        memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

        m_VertexBuffer->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        //



        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_TextureA->Bind();
        m_TextureB->Bind(1);

        Renderer renderer;
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
    }
    
    void TestDynamicBatching::OnImGuiRender() 
    {
        ImGui::SliderFloat3("Trans s", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::DragFloat2("Quad1 Position", m_Quad1Position, 10.0f);
        ImGui::DragFloat2("Quad2 Position", m_Quad2Position, 10.0f);
    }
    
    std::array<Vertex, 4> TestDynamicBatching::CreateQuad(float x, float y, float textureID) 
    {
        float size = 100.0f;

        Vertex v0;
        v0.Position = {x, y, 0.0f};
        v0.Color = {0.18f, 0.6f, 0.96f, 1.0f};
        v0.TexCoords = {0.0f, 0.0f};
        v0.TexID = textureID;

        Vertex v1;
        v1.Position = {x + size, y, 0.0f};
        v1.Color = {0.18f, 0.6f, 0.96f, 1.0f};
        v1.TexCoords = {1.0f, 0.0f};
        v1.TexID = textureID;

        Vertex v2;
        v2.Position = {x + size, y + size, 0.0f};
        v2.Color = {0.18f, 0.6f, 0.96f, 1.0f};
        v2.TexCoords = {1.0f, 1.0f};
        v2.TexID = textureID;

        Vertex v3;
        v3.Position = {x, y + size, 0.0f};
        v3.Color = {0.18f, 0.6f, 0.96f, 1.0f};
        v3.TexCoords = {0.0f, 1.0f};
        v3.TexID = textureID;

        return { v0, v1, v2, v3 };
    }
}