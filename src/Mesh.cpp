#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
    : m_Vertices(vertices), m_Indices(indices)
{
    m_VAO = new VertexArray();
    m_VAO->Bind();
    m_IndexBuffer = new IndexBuffer(m_Indices);
    m_VertexBuffer = new VertexBuffer(m_Vertices);
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VertexBuffer, layout);
}

Mesh::~Mesh() 
{
    
}

void Mesh::AddTexture(const std::string& path, unsigned int slot, const char* texType, GLenum format, GLenum pixelType) 
{
    m_Textures.push_back(new Texture(path, slot, texType, format, pixelType));
}

void Mesh::AddTexture(Texture *newTexture) 
{
    m_Textures.push_back(newTexture);
}

void Mesh::Draw
(
	Shader& shader, 
	Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation, 
	glm::quat rotation, 
	glm::vec3 scale
) 
{
    shader.Bind();
    m_VAO->Bind();
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;
    glm::vec3 camPos = camera.GetPosition();

    Renderer renderer;

    for (unsigned int i = 0; i < m_Textures.size(); i++){
        std::string num;
        std::string type = m_Textures[i]->GetType();

        if (type == "diffuse"){
            num = std::to_string(numDiffuse++);
        }
        else if (type == "specular"){
            num = std::to_string(numSpecular++);
        }

        m_Textures[i]->SetUnit(shader, (type + num), i);
        m_Textures[i]->Bind();
    }

    shader.SetUniform3f("u_CameraPosition", camPos.x, camPos.y, camPos.z);
    camera.Matrix(shader, "u_CameraMatrix");

    glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Transform the matrices to their correct form
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

    shader.SetUniformMat4f("u_Translation", trans);
    shader.SetUniformMat4f("u_Rotation", rot);
    shader.SetUniformMat4f("u_Scale", sca);
    shader.SetUniformMat4f("u_ModelMatrix", matrix);

    renderer.Draw(*m_VAO, *m_IndexBuffer, shader);
}
