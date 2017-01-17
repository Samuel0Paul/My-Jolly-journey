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

#define _NDEBUG

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

namespace mylib
{

GLuint compileShader(const char *filepath, const GLenum &SHADER_TYPE)
{
    GLuint shader = glCreateShader(SHADER_TYPE);
    std::ifstream fs(filepath);
    std::stringstream srcs;
    std::string src;

    if (fs.is_open()) {
        std::string tmp;
        while (fs) {
            std::getline(fs, tmp);
            src += tmp + "\n";
            tmp.clear();
        }
    }
    else {
        throw std::runtime_error("mylib::compileShader(): can't open shaderSrc file");
    }
#ifndef _NDEBUG
    std::clog << "mylib::compileShader(): src: \n" << src << std::endl;
#endif
    const GLchar* srcptr = src.c_str();
    glShaderSource(shader, 1, &srcptr, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR Compiling shader src: \n" << infoLog << std::endl;
        throw std::runtime_error(
            "mylib::compileShader(): glCompileShader error"
        );
    }

    return shader;
}

GLuint linkShaderProgram(std::vector<GLuint> shaders)
{
    GLuint program = glCreateProgram();
    for (const GLuint& shader: shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(program, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR Compiling shader src: \n" << infoLog << std::endl;
        throw std::runtime_error(
            "mylib::compileShader(): glCompileShader error"
        );
    }

    for (const auto& shader: shaders) {
        glDeleteShader(shader);
    }
    return program;
}
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
    if (!glfwInit())
    {
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
    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK)
    {
        std::exit(EXIT_FAILURE);
    }

    std::cout << "started window\n";

    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    GLuint vertex_shader = mylib::compileShader(
        "/home/sam/workspace/cpp/ogl_playground/01/simple.vertex.glsl",
        GL_VERTEX_SHADER
    );
    GLuint frag_shader = mylib::compileShader(
        "/home/sam/workspace/cpp/ogl_playground/01/simple.fragment.glsl",
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
        // set vertex attrib pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    // reset bindings
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}
