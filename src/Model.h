#pragma once

#include "vendor/nlohmann/json/json.hpp"

#include "Mesh.h"

using json = nlohmann::json;

std::string get_file_contents(const char* filename);

class Model
{
public:
    Model(const char* file);
    ~Model();

    void Draw(Shader& shader, Camera& camera);
private:
    const char* m_File;
    std::vector<unsigned char> data;
    json JSON;

	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

    std::vector<Mesh> meshes;

    std::vector<std::string> loadedTexName;
    std::vector<Texture> loadedTex;

    void loadMesh(unsigned int indMesh);

    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    std::vector<unsigned char> getData();
    std::vector<float> getFloats(json accessor);
    std::vector<unsigned int> getIndices(json accessor);
    std::vector<Texture> getTextures();

    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);

    std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);

};