#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <cstdlib>
#include <cstdio>

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
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

int main(void)
{
    GLFWwindow *window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::fprintf(stderr, "ERROR: glfwInit()\n");
        std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        std::fprintf(stderr, "ERROR: glewInit()\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK) {
        std::exit(EXIT_FAILURE);
    }

    GLuint VBO{0}, VAO{0};

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    std::string vshader_src{};
    {
        std::FILE* file = std::fopen(
            "/home/sam/workspace/cpp/ogl_playground/01/simple.vertex.glsl",
            "r"
        );
        GLchar* tmp = new GLchar[256];
        std::fread(&tmp, sizeof(tmp[0]), 256, file);
        std::string tmps{tmp};
        vshader_src.append(tmps);
        delete[] tmp;
    }
#ifndef _NDEBUG
    std::clog << "vertex shader src: " << vshader_src << std::endl;
#endif
    glShaderSource(vertex_shader, 1, &vshader_src.c_str(), NULL);
    glCompileShader(vertex_shader);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}
