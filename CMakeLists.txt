cmake_minimum_required(VERSION 3.5)

project(ogl_playground CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

set(ALL_INCLUDE_DIR 
        ${OPENGL_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
)

set(ALL_LIBS 
        ${OPENGL_LIBRARY}
        ${GLEW_LIBRARY}
)

message("INC DIR: \t" ${ALL_INCLUDE_DIR})
message("ALL LIBS: \t" ${ALL_LIBS})

add_executable(app_01.app ./01/main.cpp)

target_include_directories(app_01.app PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(app_01.app PUBLIC ${ALL_LIBS} glfw)

