#ifndef __MYLIB_HPP__
#define __MYLIB_HPP__

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

#endif