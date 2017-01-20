// indexed drawing using EBO

#define _NDEBUG

#include "../lib/mylib.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <exception>

#include <cstdlib>
#include <cstdio>
#include <cmath>

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

class myApp: public mylib::App
{
public:
    myApp(mylib::Window&& window)
    : mylib::App(std::move(window))
    {
    }

    ~myApp()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void startup() override
    {
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
        shader_program = mylib::linkShaderProgram(
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glUseProgram(shader_program);
        vertexColorLoc = glGetUniformLocation(shader_program, "ourColor");
    #ifndef _NDEBUG
        std::clog << "vertexColorLoc: (uniform ourColor) " << vertexColorLoc << std::endl;
    #endif
    }

    void update() override
    {
        glUseProgram(shader_program);
        timeValue   = glfwGetTime();
        greenVal    = (std::sin(timeValue) / 2) + 0.5;
        glUniform4f(vertexColorLoc, 0.5f, greenVal, 0.25f, 1.0f);
    }

    void render(double time) override
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

protected:
    GLuint VBO{0}, VAO{0}, EBO{0};
    GLuint shader_program{0}, vertexColorLoc{0};
    GLfloat timeValue{0};
    GLfloat greenVal{0};
};

int main(void)
{
    mylib::Window wind;
    myApp app(std::move(wind));
    app.run(app);

    std::exit(EXIT_SUCCESS);
}
