#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "vendor/glm/gtx/rotate_vector.hpp"
#include "vendor/glm/gtx/vector_angle.hpp"

#include "Shader.h"

class Camera {
public:
    Camera(int width, int height, glm::vec3 position);
    ~Camera();

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Input(GLFWwindow* window);
    void Matrix(Shader& shader, const char* uniform);

    glm::vec3 GetPosition() const;

private:
    glm::vec3 c_Position;
    glm::vec3 c_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 c_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 c_Matrix = glm::mat4(1.0f);

    bool firstClick = true;

    int c_Width;
    int c_Height;

    float c_Speed = 0.1f;
    float sensitivity = 100.0f;
};