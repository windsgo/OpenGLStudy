add_subdirectory(util)

add_subdirectory(vendor)

aux_source_directory(. MYGL_SRCS)
print_variable(MYGL_SRCS)
add_library(mygl 
    SHARED
    ${MYGL_SRCS}
)

print_variable(STB_IMAGE_INCLUDE_DIR)
target_include_directories(mygl PUBLIC 
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR} 
    ${STB_IMAGE_INCLUDE_DIR}
    util
    .
)
target_link_libraries(mygl 
    glfw 
    ${OPENGL_LIBRARIES} 
    ${GLEW_LIBRARIES}
)

