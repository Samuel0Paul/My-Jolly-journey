
#define _NDEBUG

#include "../lib/mylib.hpp"

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <SOIL/SOIL.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>

/**
*   ##textures
*
*   coords:     (x, t, z) => (s, t, r)

*   wrapping:   GL_TEXTURE_WRAP_S =>    GL_REPEAT/_*, GL_CLAMP_TO_*

*   filtering:  GL_TEXTURE_(MIN/MAG)_FILTER =>  GL_LINEAR, GL_NEAREST

*   minimaps:   GL_TEXTURE_(MIN/MAG)_FILTER =>  GL_(LINEAR/NEAREST)_MIPMAP_(LINEAR/NEAREST)
                                    mipmap selection ^^^        filtering btw mipmaps ^^^
*
* ######################################################
*   ## Texture Units - using multiple textures
*
*   glActiveTexture(GL_TEXTURE(0/x)) // activate tex unit before binding texture
**/

GLfloat vertices[] = {
    // pos              |  color              |   tex coords
     0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,     // top right
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,     // bottom left
    -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f,     // top left
};
GLuint indices[] = {
    0, 1, 3,    // 1st triangle
    1, 2, 3,    // 2nd triangle
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
                "/home/sam/workspace/cpp/ogl_playground/03/03.vertex.glsl",
                GL_VERTEX_SHADER
            ),
            std::make_pair("/home/sam/workspace/cpp/ogl_playground/03/03.fragment.glsl",
                GL_FRAGMENT_SHADER
            ),
        });

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Pos attrib
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat),
            (GLvoid*)(0)
        );
        glEnableVertexAttribArray(0);

        // Color attrib
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat),
            (GLvoid*)(3 * sizeof(GLfloat))
        );
        glEnableVertexAttribArray(1);

        // TexCoord attrib
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat),            // 3 pos + 3 color + 2 texcoord
            (GLvoid*)(6 * sizeof(GLfloat))  // ^
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

    }

    ~myApp()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void update() override
    {
        
    }

    void render(double app_time) override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glUniform1i(glGetUniformLocation(shader.program, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(shader.program, "ourTexture2"), 1);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

protected:
    GLuint texture[2]{0}, VBO{0}, EBO{0}, VAO{0};
    mylib::Shader shader;

private:

};

int main(const int argc, const char* const argv[])
{
    mylib::Window wind;
    myApp app(std::move(wind));
    app.run(app);

    std::exit(EXIT_FAILURE);
}
