#include "Renderer.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (auto error = glGetError()) {
        std::cout << \
            FMT_BOLD << FMT_RED << "[OpenGL Error] (0x" << std::hex << error << std::dec << "):\n"  << FMT_NONE;
        std::cout << \
            "    in line " << FMT_BOLD << FMT_YELLOW << line << FMT_NONE\
            << ", calling function: " << FMT_GREEN << function << "\n" << FMT_NONE;\
        std::cout << \
            "    in file: " << FMT_BLUE << file << FMT_NONE << std::endl;
        return false;
    }
    return true;
}