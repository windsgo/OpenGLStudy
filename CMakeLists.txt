cmake_minimum_required(VERSION 3.16)

project(test1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CXX_STANDARD 20)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(src)

function(print_variable x)
    list(LENGTH ${x} len)
    if (${len} GREATER 1 AND ${len} LESS 6)
        message(STATUS "[*PRINT*] \${${x}}:")
        foreach(_x ${${x}})
            message(STATUS "          ${_x}")
        endforeach()
    else()
        message(STATUS "[*PRINT*] \${${x}}:${${x}}")
    endif()
endfunction()

print_variable(OPENGL_INCLUDE_DIR, 1)
print_variable(OPENGL_LIBRARIES)
print_variable(GLEW_INCLUDE_DIR)
print_variable(GLEW_LIBRARIES)

