cmake_minimum_required(VERSION 3.5)

project(ogl_playground CXX)


enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(exe01 "app_01.app")
set(exe02 "app_02.app")
set(exe03 "app_03.app")
set(exe04 "app_04.app")
set(exe05 "app_05.app")
set(exe06 "app_06.app")
set(exe07 "app_07.app")
set(exe08 "app_08.app")
set(exeUnderstaningCamera "app_understandingCamera.app")
set(exe09 "app_09.app")

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Boost REQUIRED)
find_package(tinyobjloader REQUIRED)

if(UNIX)
        set(SOIL_INCLUDE_DIR "/usr/include/SOIL")
elseif(WIN32)
        set(SOIL_INCLUDE_DIR "C:/Program Files/SOIL/include")
endif(UNIX)

set(SOIL_LIBRARY "SOIL")

set(ALL_INCLUDE_DIR 
        ${OPENGL_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        ${SOIL_INCLUDE_DIR}
        ${GLM_INCLUDE_DIRS}

        ${GTKMM_INCLUDE_DIRS}
)

if(WIN32)
        set(ALL_INCLUDE_DIR ${ALL_INCLUDE_DIR} "C:/Program Files/epoxy/include")
endif(WIN32)

set(ALL_LIBS 
        ${OPENGL_LIBRARY}
        ${SOIL_LIBRARY}
        #${GLEW_LIBRARY}
        glfw
        epoxy

        ${GTKMM_LIBRARIES}
)

link_directories(
        ${GTKMM_LIBRARY_DIRS}
        ${TINYOBJLOADER_LIBRARY_DIRS}
)

message(STATUS "INC DIR: \t" ${ALL_INCLUDE_DIR})
message(STATUS "ALL LIBS: \t" ${ALL_LIBS})

# EXECUTABLES
add_executable(${exe01} ./01/main.cpp)
target_include_directories(${exe01} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe01} PUBLIC ${ALL_LIBS} ${GLEW_LIBRARY})

add_executable(${exe02} ./02/main.cpp)
target_include_directories(${exe02} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe02} PUBLIC ${ALL_LIBS})

add_executable(${exe03} ./03/main.cpp)
target_include_directories(${exe03} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe03} PUBLIC ${ALL_LIBS})

add_executable(${exe04} ./04/main.cpp)
target_include_directories(${exe04} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe04} PUBLIC ${ALL_LIBS})

add_executable(${exe05} ./05/main.cpp)
target_include_directories(${exe05} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe05} PUBLIC ${ALL_LIBS})

add_executable(${exe06} ./06/main.cpp)
target_include_directories(${exe06} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe06} PUBLIC ${ALL_LIBS})

add_executable(${exe07} ./07/main.cpp)
target_include_directories(${exe07} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe07} PUBLIC ${ALL_LIBS})

add_executable(${exe08} ./08/main.cpp)
target_include_directories(${exe08} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exe08} PUBLIC ${ALL_LIBS})

add_executable(${exeUnderstaningCamera} ./understanding_camera/main.cpp)
target_include_directories(${exeUnderstaningCamera} PUBLIC ${ALL_INCLUDE_DIR})
target_link_libraries(${exeUnderstaningCamera} PUBLIC ${ALL_LIBS})

add_executable(${exe09} ./09/main.cpp)
target_include_directories(${exe09} PUBLIC ${ALL_INCLUDE_DIR} ${TINYOBJLOADER_INCLUDE_DIRS})
target_link_libraries(${exe09} PUBLIC ${ALL_LIBS} ${TINYOBJLOADER_LIBRARIES})


# TESTS 
find_package(Boost)
if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})

        set(testexe04 "test_04.app")

        add_test(test04 ${testexe04})

        add_executable(${testexe04} ./04/test.cpp)
        target_include_directories(${testexe04} PUBLIC ${ALL_INCLUDE_DIR})
        target_link_libraries(${testexe04} PUBLIC ${ALL_LIBS} ${Boost_LIBRARIES})
endif()
