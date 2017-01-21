
#define _NDEBUG

#include "../lib/mylib.hpp"

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <SOIL/SOIL.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>

/*
*   ## MVP

*   V_clip = (M_projection * M_view * M_model) * V_local
*/


GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

glm::vec3 cubePos[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),        //1
    glm::vec3(2.0f, 5.0f, 15.0f),       //2
    glm::vec3(7.0f, -4.0f, -15.0f),     //3
    glm::vec3(6.0f, -9.0f, 0.0f),       //4
    glm::vec3(2.6f, 5.0f, -9.0f),       //5
    glm::vec3(0.0f, 7.0f, 0.0f),        //6
    glm::vec3(-9.0f, 0.0f, 0.0f),       //7
    glm::vec3(-15.0f, 0.0f, 0.0f),      //8
    glm::vec3(5.0f, 0.0f, 0.0f),        //9
    glm::vec3(0.0f, 0.0f, 0.0f),        //10
    glm::vec3(5.0f, -15.0f, 0.0f),      //11
    glm::vec3(0.0f, 0.0f, 0.0f),        //12
    glm::vec3(0.0f, 50.0f, 5.0f),        //13
};

class myApp: public mylib::App
{
public:
    
    myApp(mylib::Window&& window): 
        mylib::App(std::move(window))
    {
        
    }

    void startup() override
    {
        shader = mylib::Shader({
            std::make_pair(
                "/home/sam/workspace/cpp/ogl_playground/05/05.vertex.glsl",
                GL_VERTEX_SHADER
            ),
            std::make_pair(
                "/home/sam/workspace/cpp/ogl_playground/04/04.fragment.glsl",
                GL_FRAGMENT_SHADER
            ),
        });

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Pos attrib
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT, GL_FALSE,
            5 * sizeof(GLfloat),
            (GLvoid*)(0)
        );
        glEnableVertexAttribArray(0);

/*
        // Color attrib
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat),
            (GLvoid*)(3 * sizeof(GLfloat))
        );
        glEnableVertexAttribArray(1);
*/
        // TexCoord attrib
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT, GL_FALSE,
            5 * sizeof(GLfloat),            // 3 pos + 2 texcoord
            (GLvoid*)(3 * sizeof(GLfloat))  // ^
        );
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);   // unbind VAO

        glGenTextures(2, texture);

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        // wrap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // data
        int height{0}, width{0};
        unsigned char* image = SOIL_load_image(
            "/home/sam/workspace/cpp/ogl_playground/03/assets/drawing.png",
            &width, &height, 0, SOIL_LOAD_RGB
        );
        glTexImage2D(
            GL_TEXTURE_2D,      // target => GL_TEXTURE_2D (not 1D and 3D)
            0,                  // mipmap level, 0 is base level
            GL_RGB,             // storing format
            width,              
            height,
            0,                  // (some legacy shit)
            GL_RGB,             // format of i/p being read
            GL_UNSIGNED_BYTE,   // type of i/p
            image               // i/p image data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        // texture 02
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        image = SOIL_load_image(
            "/home/sam/workspace/cpp/ogl_playground/03/assets/jesus.png",
            &width, &height, 0, SOIL_LOAD_RGB
        );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
            GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        glEnable(GL_DEPTH_TEST);
        
    }

    ~myApp()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void update(double time) override
    {
        model = glm::rotate(
            glm::mat4(), 
            //glm::radians(-55.0f),
            (GLfloat) glfwGetTime() * 1.0f,
            glm::vec3(1.0f, 0.2f, 0.5f)
        );
        view = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(
            45.0f, 
            static_cast<GLfloat>(this->getWindowWidth()) / this->getWindowHeight(),
            0.1f, 100.0f
        );

        shader.use();
        /*glUniformMatrix4fv(
            glGetUniformLocation(shader.program, "model"),
            1, GL_FALSE, glm::value_ptr(model)
        );*/
        glUniformMatrix4fv(
            glGetUniformLocation(shader.program, "view"),
            1, GL_FALSE, glm::value_ptr(view)
        );
        glUniformMatrix4fv(
            glGetUniformLocation(shader.program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection)
        );
    }

    void render() override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glUniform1i(glGetUniformLocation(shader.program, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(shader.program, "ourTexture2"), 1);
        
        for (GLuint i = 0; i < 13; ++i) {
            model = glm::translate(glm::mat4(), cubePos[i]);
            model = glm::rotate(model, 20.0f * i, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(
                glGetUniformLocation(shader.program, "model"),
                1, GL_FALSE, glm::value_ptr(model)
            );
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
    }

protected:
    GLuint texture[2]{0}, VBO{0}, VAO{0};
    mylib::Shader shader;
    glm::mat4 trans;
    glm::mat4 model, view, projection;

private:

};

int main(const int argc, const char* const argv[])
{
    mylib::Window wind;
    myApp app(std::move(wind));
    app.run(app);

    std::exit(EXIT_SUCCESS);
}
