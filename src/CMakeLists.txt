add_executable(test1 test1.cpp Renderer.cpp VertexBuffer.cpp IndexBuffer.cpp VertexArray.cpp VertexBufferLayout.cpp)
target_include_directories(test1 PUBLIC ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} .)
target_link_libraries(test1 glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

