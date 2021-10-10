#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
http://docs.gl/
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Debug.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestBatchRenderingColor.h"
#include "tests/TestBatchRenderingTexture.h"
#include "tests/TestPyramid.h"
#include "tests/TestPlane.h"

#define WIDTH 800
#define HEIGHT 800


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        cout << "Error glewInit()" << endl;
        return 1;
    }

    cout << glGetString(GL_VERSION) << endl;
    {
        GLCall(glEnable(GL_DEPTH_TEST));


        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::Test* currentTest = NULL;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
        testMenu->RegisterTest<test::TestBatchRenderingColor>("Batch Rendering Color");
        testMenu->RegisterTest<test::TestBatchRenderingTexture>("Batch Rendering Texture");
        testMenu->RegisterTest<test::TestPyramid>("Pyramid", window);
        testMenu->RegisterTest<test::TestPlane>("Plane", window);

        GLCall(glViewport(0,0, WIDTH, HEIGHT));

        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();
            if(currentTest){
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();

                if (currentTest != testMenu && glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS){
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
            }
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
