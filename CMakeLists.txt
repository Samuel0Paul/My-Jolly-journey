cmake_minimum_required(VERSION 3.5)

project(OGL_Playground CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

set(${glfw} glfw)

message("OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")
message("GLEW_LIBRARIES: ${GLEW_LIBRARIES}")
message("GLFW_LIBRARIES: ${GLFW_LIBRARIES}")

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
)

add_executable(01_app ./01/main.cpp)
target_link_libraries(01_app ${ALL_LIBS})

