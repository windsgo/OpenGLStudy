#pragma once

#include <GL/glew.h>
#include <iostream>

#define FMT_NONE    "\033[0m"
#define FMT_RED     "\033[31m"
#define FMT_GREEN   "\033[32m"
#define FMT_YELLOW  "\033[33m"
#define FMT_BLUE    "\033[34m"
#define FMT_BOLD    "\033[1m"

#define ASSERT(x) \
    if (!(x)) {\
        std::cout \
            << FMT_BOLD << FMT_RED << "ASSERTION FAILED!" << FMT_NONE \
            << " -> ASSERT(" << FMT_RED << #x << FMT_NONE << ")\n";\
        std::cout \
            << "    in line " << FMT_BOLD << FMT_YELLOW << __LINE__ << FMT_NONE\
            << ", " << __PRETTY_FUNCTION__ << std::endl;\
        exit(1);\
    }

#define GLCall(x) \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);