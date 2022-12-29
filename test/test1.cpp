#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "Shader.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

int main(int argc, char *argv[])
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cout << "glfwInit()" << FMT_BOLD << FMT_RED << " failed" << FMT_NONE << std::endl;
        return -1;
    }
    else
    {
        std::cout << "glfwInit()" << FMT_BOLD << FMT_GREEN << " success" << FMT_NONE << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "glfwCreateWindow()" << FMT_BOLD << FMT_RED << " failed" << FMT_NONE << std::endl;
        return -1;
    }
    else
    {
        std::cout << "glfwCreateWindow()" << FMT_BOLD << FMT_GREEN << " success" << FMT_NONE << std::endl;
    }

    // make openGL context
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // init glew (wraper of gl funcs) after glfw makes openGL context
    auto ret = glewInit();
    std::cout << "glewInit() does" << (ret == GLEW_OK ? "" : "n't") << " return GLEW_OK" << std::endl;
    if (ret != GLEW_OK)
    {
        std::cout << "glewInit()" << FMT_BOLD << FMT_RED << " failed" << FMT_NONE << std::endl;
    }
    else
    {
        std::cout << "glewInit()" << FMT_BOLD << FMT_GREEN << " success" << FMT_NONE << std::endl;
    }

    std::cout << "OpenGL Version: " << FMT_BOLD << FMT_GREEN << glGetString(GL_VERSION) << FMT_NONE << std::endl;

    // draw

    {
        float positions[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f};

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0};

        // allocate on stack 
        // automatically destructed at end of the scope

        VertexArray va;
        VertexBuffer vb{positions, 4 * 2 * sizeof(float)}; // data, size
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib{indices, 2 * 3}; // data, count

        Shader shader{"res/shaders/Basic.shader"};
        shader.Bind();
        shader.SetUniform4f("u_Color", 1, 1, 0, 1);

        va.UnBind();
        ib.UnBind();
        vb.UnBind();
        shader.UnBind();

        float r = 0.0f;
        float increment = 0.01f;

        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // do things below

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 1, 0, 0.6);

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // using 6 indices

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