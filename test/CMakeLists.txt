add_executable(test1 
    test1.cpp
)
target_include_directories(test1 PUBLIC 
    ${OPENGL_INCLUDE_DIR} 
    ${GLEW_INCLUDE_DIR} 
    ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(test1 
    mygl 
    glfw 
    ${OPENGL_LIBRARIES} 
    ${GLEW_LIBRARIES}
)

