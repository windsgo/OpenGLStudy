#include "Shader.h"
#include "Renderer.h"

#include <fstream>
#include <sstream>

Shader::Shader(const std::string &filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string &filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            if (type == ShaderType::NONE)
                continue;
            if (line.size() != 0)
                ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // iv, i:integer, v:vector
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << FMT_BOLD << FMT_RED << "[Compile Error] " << FMT_NONE << FMT_YELLOW << "Fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n"
                  << FMT_NONE;
        std::cout << FMT_BOLD << FMT_YELLOW << "    Error msg: " << FMT_NONE << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
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

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}
void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

// set uniforms
void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string &name)
{

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache.at(name);
    }

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << FMT_BOLD << FMT_YELLOW << "[Warning] " << FMT_NONE
                  << FMT_YELLOW << "uniform variable " << FMT_NONE
                  << FMT_BOLD << FMT_BLUE << name << FMT_NONE
                  << FMT_YELLOW " does not exist!" << FMT_NONE << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}