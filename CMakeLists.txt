cmake_minimum_required(VERSION 3.5)

project(ogl_playground CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(exe01 "app_01.app")
set(exe02 "app_02.app")

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
        #${GLEW_LIBRARY}
        glfw
        epoxy
)

message("INC DIR: \t" ${ALL_INCLUDE_DIR})
message("ALL LIBS: \t" ${ALL_LIBS})

add_executable(${exe01} ./01/main.cpp)
target_include_directories(${exe01} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe01} PUBLIC ${ALL_LIBS} ${GLEW_LIBRARY})

add_executable(${exe02} ./02/main.cpp)
target_include_directories(${exe02} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe02} PUBLIC ${ALL_LIBS})

