#include "TestPlane.h"
#include "../Debug.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/imgui/imgui.h"

#define WIDTH 800
#define HEIGHT 800


namespace test
{
    TestPlane::TestPlane(GLFWwindow* window)
        : m_Window(window)
    {
        // Vertices coordinates
        Vertex vertices[] = 
        {
            Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
        };

        // Indices for vertices order
        unsigned int indices[] =
        {
            0, 1, 2,
            0, 2, 3
        };

        Vertex lightVertices[] =
        { //     COORDINATES     //
            Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
            Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
            Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
            Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
            Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
            Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
            Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
            Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
        };

        unsigned int lightIndices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 4, 7,
            0, 7, 3,
            3, 7, 6,
            3, 6, 2,
            2, 6, 5,
            2, 5, 1,
            1, 5, 4,
            1, 4, 0,
            4, 5, 6,
            4, 6, 7
        };
        //auto quad0 = CreateQuad(-0.5, -0.5, 0.0);

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // THIS IS FOR OBJECT //
        std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
        std::vector<unsigned int> ind(indices, indices + sizeof(indices) / sizeof(unsigned int));

        m_Floor = new Mesh(verts, ind);
        //m_Floor->AddTexture("res/textures/planks.png", 0, "diffuse", GL_RGBA, GL_UNSIGNED_BYTE);
        m_FloorTexture = new Texture("res/textures/planks.png", 0, "diffuse", GL_RGBA, GL_UNSIGNED_BYTE);
        m_Floor->AddTexture(m_FloorTexture);
        m_Floor->AddTexture("res/textures/planksSpec.png", 1, "specular", GL_RED, GL_UNSIGNED_BYTE);


        //------
        
        /*
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(verts);
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(ind);

        m_Shader = std::make_unique<Shader>("res/shaders/stream.shader");

        m_Texture = std::make_unique<Texture>("res/textures/planks.png", 0, GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE);
        m_SpecularTexture = std::make_unique<Texture>("res/textures/planksSpec.png", 1, GL_TEXTURE_2D, GL_RGB, GL_UNSIGNED_BYTE);
        */

        //m_Texture = std::make_unique<Texture>("res/textures/planks.png", 0, GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE);
        //m_SpecularTexture = std::make_unique<Texture>("res/textures/planksSpec.png", 1, GL_TEXTURE_2D, GL_RGB, GL_UNSIGNED_BYTE);
        

        m_Shader = std::make_unique<Shader>("res/shaders/stream.shader");
        
        m_Camera = std::make_unique<Camera>(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
        /*----------------------------*/


        // THIS IS FOR LIGHT SOURCE //
        std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
        std::vector<unsigned int> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(unsigned int));

        m_LightSource = new Mesh(lightVerts, lightInd);

        //---
        
        /*
        m_LightVAO = std::make_unique<VertexArray>();
        
        m_LightVertexBuffer = std::make_unique<VertexBuffer>(lightVerts);
        VertexBufferLayout LightLayout;
        LightLayout.Push<float>(3);
        m_LightVAO->AddBuffer(*m_LightVertexBuffer, LightLayout);

        m_LightIndexBuffer = std::make_unique<IndexBuffer>(lightInd);

        m_LightShader = std::make_unique<Shader>("res/shaders/light.shader");
        m_LightShader->Bind();

        m_LightVAO->Unbind();
        m_LightVertexBuffer->Unbind();
        m_LightIndexBuffer->Unbind();
        */
        

        m_LightShader = std::make_unique<Shader>("res/shaders/light.shader");

        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        /*--------------------------*/

        std::cout << "after" << std::endl;

        // POSITIONS OF OBJECT AND LIGHT SOURCE //
        glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 objectModel = glm::mat4(1.0f);
        objectModel = glm::translate(objectModel, objectPos);

        glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        /*---------------------------------------*/

        m_LightShader->Bind();
        m_LightShader->SetUniformMat4f("u_ModelMatrix", lightModel);
        m_LightShader->SetUniform4f("u_LightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_ModelMatrix", objectModel);
        m_Shader->SetUniform4f("u_LightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        m_Shader->SetUniform3f("u_LightPosition", lightPos.x, lightPos.y, lightPos.z);

    }
    
    TestPlane::~TestPlane() 
    {
        
    }
    
    void TestPlane::OnUpdate(float deltaTime) 
    {
        
    }
    
    void TestPlane::OnRender() 
    {
        GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


        /*
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(t_Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(800/800), 0.1f, 100.0f);

        glm::mat4 mvp = proj * view * model;

        m_Shader->SetUniformMat4f("u_MVP", mvp);
        */

        m_Camera->Input(m_Window);
        m_Camera->updateMatrix(45.0f, 0.1f, 100.0f);

        
        //Renderer renderer;

        //m_Texture->Bind();
        //m_SpecularTexture->Bind();
        
        /*
        {
            m_Shader->Bind();
            glm::vec3 camPos = m_Camera->GetPosition();
            m_Shader->SetUniform3f("u_CameraPosition", camPos.x, camPos.y, camPos.z);
            m_Camera->Matrix(*m_Shader, "u_CameraMatrix");
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            m_LightShader->Bind();
            m_Camera->Matrix(*m_LightShader, "u_CameraMatrix");
            renderer.Draw(*m_LightVAO, *m_LightIndexBuffer, *m_LightShader);
        }
        */

        m_Floor->Draw(*m_Shader, *m_Camera);
        m_LightSource->Draw(*m_LightShader, *m_Camera);

    }
    
    void TestPlane::OnImGuiRender() 
    {
        
    }
    
    void TestPlane::SetWindow(GLFWwindow* window) 
    {
        m_Window = window;
    }

    /*
    std::array<Vertex, 4> TestPlane::CreateQuad(float x, float y, float textureID) 
    {
        float size = 1.0f;

        Vertex v0;
        v0.Position = {x, y, 0.0f};
        v0.Color = {1.0f, 0.0f, 0.0f, 1.0f};
        v0.TexCoords = {0.0f, 0.0f};
        v0.TexID = textureID;

        Vertex v1;
        v1.Position = {x + size, y, 0.0f};
        v1.Color = {0.0f, 1.0f, 0.0f, 1.0f};
        v1.TexCoords = {1.0f, 0.0f};
        v1.TexID = textureID;

        Vertex v2;
        v2.Position = {x + size, y + size, 0.0f};
        v2.Color = {0.0f, 0.0f, 1.0f, 1.0f};
        v2.TexCoords = {1.0f, 1.0f};
        v2.TexID = textureID;

        Vertex v3;
        v3.Position = {x, y + size, 0.0f};
        v3.Color = {1.0f, 1.0f, 1.0f, 1.0f};
        v3.TexCoords = {0.0f, 1.0f};
        v3.TexID = textureID;

        return { v0, v1, v2, v3 };
    }
    */
}