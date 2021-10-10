#include "Model.h"

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

Model::Model(const char* file) 
    : m_File(file)
{
    std::string text = get_file_contents(m_File);
    JSON = json::parse(text);

    data = getData();

}

Model::~Model() 
{
    
}

void Model::Draw(Shader& shader, Camera& camera) 
{
    for (unsigned int i = 0; i < meshes.size(); i++){
        meshes[i].Mesh::Draw(shader, camera);
    }
    
}

void Model::loadMesh(unsigned int indMesh) 
{
    unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

    std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
    std::vector<glm::vec3> positions = groupFloatsVec3(posVec);

    std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
    std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);

    std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
    std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

    std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
    std::vector<unsigned int> indices = getIndices(JSON["accessors"][indAccInd]);
    std::vector<Texture> textures = getTextures();

    Mesh newMesh(vertices, indices);
    for(unsigned int i = 0; i < textures.size(); i++){
        newMesh.AddTexture(&textures[i]);
    }
    meshes.push_back(newMesh);
}

std::vector<unsigned char> Model::getData() 
{
    std::string bytesText;
    std::string uri = JSON["buffers"][0]["uri"];

    std::string fileStr = std::string(m_File);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of("/") + 1);
    bytesText = get_file_contents((fileDirectory + uri).c_str());

    std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
}

std::vector<float> Model::getFloats(json accessor) 
{
    std::vector<float> floatVec;

    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int numPerVert;

    if (type == "SCALAR") numPerVert = 1;
    else if (type == "VEC2") numPerVert = 2;
    else if (type == "VEC3") numPerVert = 3;
    else if (type == "VEC4") numPerVert = 4;
    else throw std::invalid_argument("Invalid argument.");

    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert;

    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i){
        unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++]};
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }

    return floatVec;
}

std::vector<unsigned int> Model::getIndices(json accessor) 
{
    std::vector<unsigned int> indices;

    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];

    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int beginningOfData = byteOffset + accByteOffset;

    if (componentType == 5125){
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i){
        unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++]};
        unsigned int value;
        std::memcpy(&value, bytes, sizeof(unsigned int));
        indices.push_back((unsigned int)value);
        }
    }
    if (componentType == 5123){
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i){
        unsigned char bytes[] = { data[i++], data[i++]};
        unsigned short value;
        std::memcpy(&value, bytes, sizeof(unsigned short));
        indices.push_back((unsigned int)value);
        }
    }
    if (componentType == 5125){
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i){
        unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++]};
        short value;
        std::memcpy(&value, bytes, sizeof(short));
        indices.push_back((unsigned int)value);
        }
    }

    return indices;
}

std::vector<Texture> Model::getTextures() 
{
    std::vector<Texture> textures;

    std::string fileStr = std::string(m_File);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of("/") + 1);

    unsigned unit = 0;

    for (unsigned int i = 0; i < JSON["images"].size(); i++){
        std::string texPath = JSON["images"][i]["uri"];

        bool skip = false;
        for (unsigned int j = 0; j < loadedTexName.size(); j++){
            if(loadedTexName[j] == texPath){
                textures.push_back(loadedTex[j]);
                skip = true;
                break;
            }
        }


        // THIS IS NOT IDEAL AT ALL, NEEDS TO BE CHANGED - METALLIC ROUGHNESS IS FOR PBR
        if (!skip){
            if (texPath.find("baseColor") != std::string::npos){
                Texture diffuse = Texture(fileDirectory + texPath, loadedTex.size(), "diffuse");
                textures.push_back(diffuse);
                loadedTex.push_back(diffuse);
                loadedTexName.push_back(texPath);
            }
            else if (texPath.find("metallicRoughness") != std::string::npos){
                Texture specular = Texture(fileDirectory + texPath, loadedTex.size(), "specular");
                textures.push_back(specular);
                loadedTex.push_back(specular);
                loadedTexName.push_back(texPath);
            }
        }
        
    }
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec) 
{
    std::vector<glm::vec2> vectors;
    for (int i = 0; i < floatVec.size(); i){
        vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec) 
{
    std::vector<glm::vec3> vectors;
    for (int i = 0; i < floatVec.size(); i){
        vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec) 
{
    std::vector<glm::vec4> vectors;
    for (int i = 0; i < floatVec.size(); i){
        vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs) 
{
    std::vector<Vertex> vertices;

    for (int i = 0; i < positions.size(); i++){
        vertices.push_back
        (
            Vertex
            {
                positions[i],
                normals[i],
                glm::vec3(1.0f, 1.0f, 1.0f),
                texUVs[i]
            }
        );
    }
}

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}
