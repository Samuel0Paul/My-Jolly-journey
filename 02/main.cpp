// indexed drawing using EBO

#define _NDEBUG

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <exception>

#include <cstdlib>
#include <cstdio>

#include "../lib/mylib.hpp"

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLfloat vertices[] = {
    0.5f, 0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f,  // top left
};
GLuint indices[] = {    //NOTE: starts from `0`
    0, 1, 3,    // 1st triangle
    1, 2, 3,    // 2nd triangle
};

int main(void)
{
    GLFWwindow *window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        std::fprintf(stderr, "ERROR: glfwInit()\n");
        std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        std::fprintf(stderr, "ERROR: glfw window init err\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK) {
        std::fprintf(stderr, "ERROR: glewInit()\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    std::cout << "started window\n";

    GLuint VBO{0}, VAO{0}, EBO{0};

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    GLuint vertex_shader = mylib::compileShader(
        "/home/sam/workspace/cpp/ogl_playground/02/color.vertex.glsl",
        GL_VERTEX_SHADER
    );
    GLuint frag_shader = mylib::compileShader(
        "/home/sam/workspace/cpp/ogl_playground/02/color.fragment.glsl",
        GL_FRAGMENT_SHADER
    );
    GLuint shader_program = mylib::linkShaderProgram(
        {
            vertex_shader,
            frag_shader,
        }
    );
    
    glBindVertexArray(VAO);
        // copy vertices into ogl buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // copy index array in ogl element buffer EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // set vertex attrib pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    // reset bindings
    glBindVertexArray(0);   //!NOTE unbind VAO first to make it stop recording unbinds
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}
