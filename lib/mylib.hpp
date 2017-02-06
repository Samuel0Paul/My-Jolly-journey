#ifndef __MYLIB_HPP__
#define __MYLIB_HPP__

#include <epoxy/gl.h>
#ifdef _WIN32
#include <epoxy\wgl.h>
#else
#include <epoxy/glx.h>
#endif
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
			throw std::runtime_error("glfwInit() returned error");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _windowHndl = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!_windowHndl) {
            std::cerr << "ERROR: (" << __PRETTY_FUNCTION__
                      << ") glfwCreateWindow() returned NULL"
                      << std::endl;
			throw std::runtime_error("glfwCreateWindow() returned NULL");
        }
        glfwSetKeyCallback(_windowHndl, mylib::Window::key_callback);
        glfwSetInputMode(_windowHndl, GLFW_STICKY_KEYS, 1);
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
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            // clear ESC key pressed event (stickykeyconfigred)
            glfwGetKey(window, GLFW_KEY_ESCAPE);
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
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

    static void run(App& app)
    {
        app.window.init();
        app.startup();

        while (!glfwWindowShouldClose(app.window.getWindow())) {
            app.update(glfwGetTime());
            app.render();

            glfwSwapBuffers(app.window.getWindow());
            glfwPollEvents();
        }

        app.shutdown();
    }

    static void threaded_run(void* app_addr = nullptr)
    {
        if (app_addr == nullptr) {
            std::cerr << "add_addr = nullptr" << std::endl;
			throw std::runtime_error("app address is null");
        }

        mylib::App *app = reinterpret_cast<mylib::App*>(app_addr);
        app->window.init();
        app->startup();

        while (!glfwWindowShouldClose(app->window.getWindow())) {
            app->update(glfwGetTime());
            app->render();

            glfwSwapBuffers(app->window.getWindow());
            glfwPollEvents();
        }

        app->shutdown();
    }

    virtual void startup() {}
    virtual void update(double) {}
    virtual void render() {}
    virtual void shutdown() {}

    unsigned int getWindowWidth()
    {
        return static_cast<unsigned int>(this->window.width);
    }

    unsigned int getWindowHeight()
    {
        return static_cast<unsigned int>(this->window.height);
    }

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