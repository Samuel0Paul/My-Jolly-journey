
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

/**
*   ## Identity mat
    ((1, 0, 0, 0),      (1      (1
     (0, 1, 0, 0),  *    2  =    2
     (0, 0, 1, 0),       3       3
     (0, 0, 0, 1))       4)      4)

*   ## scaling
    ((s1, 0, 0, 0),     (x      (s1.x,
     (0, s2, 0, 0), *    y  =    s2.y,
     (0, 0, s3, 0),      z       s3.z,
     (0, 0, 0, 1))       1)      1   )

*   ## translation
    ((1, 0, 0, tx),     (x      (x + tx,
     (0, 1, 0, ty), *    y  =    y + ty,
     (0, 0, 1, tz),      z       z + tz,
     (0, 0, 0, 1))       1)      1      )

*   ## ratation
    angle_in_deg = angle_in_rad * (180 / PI)

    // rot about x axis
    ((1, 0,       0,      0),     (x      (         x,
     (0, cos(o), -sin(o), 0), *    y  =    cos(o).y - sin(o).z,
     (0, sin(o),  cos(o), 0),      z       sin(o).y + cos(o).z,
     (0, 0,       0,      1))       1)              1           )
    
    // rot abt y axis math
    (( cos(o), 0, sin(o), 0),
     ( 0,      1, 0,      0),
     (-sin(o), 0, cos(o), 0),
     (0,       0, 0,      1))

    // rot abt z axis math
    ((cos(o), -sin(o), 0, 0),
     (sin(o),  cos(o), 0, 0),
     (0,       0,      1, 0),
     (0,       0,      0, 1))

*   ## combining transforms
    tans_mat = 
*
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
                "/home/sam/workspace/cpp/ogl_playground/04/04.vertex.glsl",
                GL_VERTEX_SHADER
            ),
            std::make_pair(
                "/home/sam/workspace/cpp/ogl_playground/04/04.fragment.glsl",
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

        /*
        trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        */
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 50.0f,
                    glm::vec3(0.0f, 0.0f, 1.0f));
        shader.use();
        glUniformMatrix4fv(
            glGetUniformLocation(shader.program, "transform"),
            1, GL_FALSE, glm::value_ptr(trans)
        );
        
    }

    ~myApp()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void update(double time) override
    {
        
    }

    void render() override
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
    glm::mat4 trans;

private:

};

int main(const int argc, const char* const argv[])
{
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << "vec: " << glm::to_string(vec) << std::endl;

    trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vec = trans * vec;
    std::cout << "vec: " << glm::to_string(vec) << std::endl;

    mylib::Window wind;
    myApp app(std::move(wind));
    app.run(app);

    std::exit(EXIT_SUCCESS);
}
