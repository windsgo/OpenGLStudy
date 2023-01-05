cmake_minimum_required(VERSION 3.16)

project(test1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CXX_STANDARD 20)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

function(print_variable x)
    list(LENGTH ${x} len)
    if (${len} GREATER 1 AND ${len} LESS 10)
        message(STATUS "[*PRINT*] \${${x}}:")
        foreach(_x ${${x}})
            message(STATUS "          ${_x}")
        endforeach()
    else()
        message(STATUS "[*PRINT*] \${${x}}:${${x}}")
    endif()
endfunction()

print_variable(OPENGL_INCLUDE_DIR)
print_variable(OPENGL_LIBRARIES)
print_variable(GLEW_INCLUDE_DIR)
print_variable(GLEW_LIBRARIES)

add_subdirectory(src)
add_subdirectory(test)
