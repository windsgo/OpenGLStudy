#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "GlobalFunctions.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Logger.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(int argc, char *argv[])
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        Logger->error("glfwInit() ", STR_FAILED);
        return -1;
    }
    else
    {
        Logger->info("glfwInit() ", STR_SUCCESS);
    }

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        Logger->error("glfwTerminate() ", STR_FAILED);
        return -1;
    }
    else
    {
        Logger->info("glfwTerminate() ", STR_SUCCESS);
    }

    // make openGL context
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // init glew (wraper of gl funcs) after glfw makes openGL context
    auto ret = glewInit();
    Logger->info("glewInit() does", (ret == GLEW_OK ? "" : "n't"), " return GLEW_OK");
    if (ret != GLEW_OK)
    {
        Logger->error("glewInit() ", STR_FAILED);
    }
    else
    {
        Logger->info("glewInit() ", STR_SUCCESS);
    }

    Logger->info("OpenGL Version: ", FMT_GREEN + std::string((char*)glGetString(GL_VERSION)));

    // draw

    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0};

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //透明渲染方式

        // allocate on stack 
        // automatically destructed at end of the scope

        VertexArray va;
        VertexBuffer vb{positions, 4 * 4 * sizeof(float)}; // data, size
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib{indices, 2 * 3}; // data, count

        // 投影矩阵，定义了窗口的边界坐标
        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        Shader shader{"res/shaders/Basic.shader"};
        shader.Bind();
        shader.SetUniform4f("u_Color", 1, 1, 0, 1);
        shader.SetUniformMat4f("u_MVP", proj); // 传递投影矩阵，GPU中进行计算

        Texture texture("res/textures/default256.png");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        va.UnBind();
        ib.UnBind();
        vb.UnBind();
        shader.UnBind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.01f;

        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            // do things below
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 1, 0, 0.6);

            renderer.Draw(va, ib, shader);


            if (r > 1.0f)
                increment = -0.01f;
            else if (r < 0.0f)
                increment = 0.01f;

            r += increment;

            // do things above

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}