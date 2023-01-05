add_subdirectory(util)

add_subdirectory(vendor)

aux_source_directory(. MYGL_SRCS)
print_variable(MYGL_SRCS)
add_library(mygl 
    SHARED
    ${MYGL_SRCS}
)

print_variable(STB_IMAGE_INCLUDE_DIR)
print_variable(GLM_INCLUDE_DIR)
print_variable(VENDOR_INCLUDE_DIR)

target_include_directories(mygl PUBLIC 
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR} 
    ${VENDOR_INCLUDE_DIR}
    util
    .
)
target_link_libraries(mygl 
    glfw 
    stb_image
    # glm::glm_static
    ${OPENGL_LIBRARIES} 
    ${GLEW_LIBRARIES}
)

