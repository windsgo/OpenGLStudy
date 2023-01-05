add_executable(test1 
    test1.cpp
)
target_include_directories(test1 PUBLIC 
    ${OPENGL_INCLUDE_DIR} 
    ${GLEW_INCLUDE_DIR} 
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/util
    ${VENDOR_INCLUDE_DIR}
)
target_link_libraries(test1 
    mygl 
    stb_image
    # glm::glm_static
    glfw 
    ${OPENGL_LIBRARIES} 
    ${GLEW_LIBRARIES}
)

