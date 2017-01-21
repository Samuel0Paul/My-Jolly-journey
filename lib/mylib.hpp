#ifndef __MYLIB_HPP__
#define __MYLIB_HPP__

#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <epoxy/egl.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <exception>

#include <cstdlib>
#include <cstdio>


#include "./shader.hpp"


namespace mylib
{

class Window
{
public:
    unsigned int width{800}, height{600};
    std::string title{"Window"};
    bool fullscreen{false};

    Window()
    {
        glfwSetErrorCallback(mylib::Window::error_callback);
    }

    Window(unsigned int width, unsigned int height, const char* title):
        width(width), height(height), title(title)
    {
        Window();
    }

    ~Window()
    {
        glfwDestroyWindow(_windowHndl);
    }

    void init()
    {
        if (!glfwInit()) {
            std::cerr << "ERROR: (" << __PRETTY_FUNCTION__ << ") glfwInit() returned error"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _windowHndl = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!_windowHndl) {
            std::cerr << "ERROR: (" << __PRETTY_FUNCTION__
                      << ") glfwCreateWindow() returned NULL"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }
        glfwSetKeyCallback(_windowHndl, mylib::Window::key_callback);
        glfwMakeContextCurrent(_windowHndl);
    }

    GLFWwindow* getWindow()
    {
        return this->_windowHndl;
    }

    static void error_callback(int error, const char *description)
    {
        fprintf(stderr, "Error: CODE: 0x%x , %s\n", error, description);
    }

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

protected:

private:
    GLFWwindow *_windowHndl{nullptr};

};

class App
{
public:
    mylib::Window window;

    App(mylib::Window&& window): window(window)
    {
    }
    App() = delete;

    void run(App& app)
    {
        window.init();
        app.startup();

        while (!glfwWindowShouldClose(window.getWindow())) {
            app.update(glfwGetTime());
            app.render();

            glfwSwapBuffers(window.getWindow());
            glfwPollEvents();
        }

        app.shutdown();
    }

    virtual void startup() {}
    virtual void update(double) {}
    virtual void render() {}
    virtual void shutdown() {}

protected:

private:

};

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

};

#endif