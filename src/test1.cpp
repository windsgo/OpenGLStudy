#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            if (type == ShaderType::NONE) continue;
            if (line.size() != 0)
                ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // iv, i:integer, v:vector
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << \
            FMT_BOLD << FMT_RED << "[Compile Error] " << FMT_NONE <<\
            FMT_YELLOW << "Fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n" << FMT_NONE;
        std::cout << \
            FMT_BOLD << FMT_YELLOW << "    Error msg: " << FMT_NONE << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id; 
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(auto program = glCreateProgram());
    auto vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    auto fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int
main(int argc, char *argv[])
{
    GLFWwindow* window;

    if (!glfwInit()) {
        std::cout << "glfwInit()" << FMT_BOLD << FMT_RED << " failed" << FMT_NONE << std::endl;
        return -1;
    } else {
        std::cout << "glfwInit()" << FMT_BOLD << FMT_GREEN << " success" << FMT_NONE << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cout << "glfwCreateWindow()" << FMT_BOLD << FMT_RED << " failed" << FMT_NONE << std::endl;
        return -1;
    } else {
        std::cout << "glfwCreateWindow()" << FMT_BOLD << FMT_GREEN << " success" << FMT_NONE << std::endl;
    }

    // make openGL context
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // init glew (wraper of gl funcs) after glfw makes openGL context
    auto ret = glewInit();
    std::cout << "glewInit() does" << (ret == GLEW_OK ? "" : "n't") << " return GLEW_OK" << std::endl;
    if (ret != GLEW_OK) {
        std::cout << "glewInit()" << FMT_BOLD << FMT_RED << " failed" << FMT_NONE << std::endl;
    } else {
        std::cout << "glewInit()" << FMT_BOLD << FMT_GREEN << " success" << FMT_NONE << std::endl;
    }

    std::cout << "OpenGL Version: " << FMT_BOLD << FMT_GREEN << glGetString(GL_VERSION) << FMT_NONE << std::endl;

    // draw

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    // allocate on stack is not a good idea
    // since they will be destructed after glfw context is destroyed


    // vertex array object
    VertexArray va;
    VertexBuffer vb{positions, 4 * 2 * sizeof(float)}; // data, size
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib{indices, 2 * 3}; // data, count
    

    auto source = ParseShader("res/shaders/Basic.shader");

    auto shader = CreateShader(source.VertexSource, source.FragmentSource);
    // std::cout << source.VertexSource << std::endl;
    // std::cout << source.FragmentSource << std::endl;
    

    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 1, 1, 0, 0.6));

    float r = 0.0f;
    float increment = 0.01f;

    while (!glfwWindowShouldClose(window)) 
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        // do things below

        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 1, 0, 0.6));

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

    GLCall(glDeleteProgram(shader));

    glfwTerminate();

    return 0;
}