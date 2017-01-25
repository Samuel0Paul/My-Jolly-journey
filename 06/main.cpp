
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

    * OpenGL: a formal specification of a graphics API that defines the layout and
            output of each function.

    * GLEW: an extension loading library that loads and sets all OpenGL’s function pointers
            for us so we can use all (modern) OpenGL’s functions.

    * Viewport: the window where we render to.
            Graphics Pipeline: the entire process a vertex has to walk through before ending up
            as a pixel on your screen.

    * Shader: a small program that runs on the graphics card. Several stages of the graphics
            pipeline can use user-made shaders to replace existing functionality.

    * Vertex: a collection of data that represent a single point.

    * Normalized Device Coordinates: the coordinate system your vertices end up in
            after clipping and perspective division is performed on clip coordinates. 
            All vertex positions in NDC between -1.0 and 1.0 will not be discarded and
            end up visible.

    * Vertex Buffer Object: a buffer object that allocates memory and stores all the vertex
            data for the graphics card to use.
            
    * Vertex Array Object: stores buffer and vertex attribute state information.

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
    glm::vec3( 0.0f, 0.0f, 0.0f),
    glm::vec3( 2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f),
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

        shader.use();
        modelLoc        = glGetUniformLocation(shader.program, "model");
        viewLoc         = glGetUniformLocation(shader.program, "view");
        projectionLoc   = glGetUniformLocation(shader.program, "projection");

        cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

        // mouse setup
        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        sensitivity = 0.05f;
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        lastX = static_cast<GLfloat>(xpos);
        lastY = static_cast<GLfloat>(-ypos); // reversed y cord, botton to top

        glEnable(GL_DEPTH_TEST);
        
    }

    ~myApp()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void update(double time) override
    {
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;
        cameraSpeed = 0.5f * deltaTime;

        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        xoffset = static_cast<GLfloat>(xpos - lastX) * sensitivity;
        // reversed y cord, botton to top
        yoffset = static_cast<GLfloat>(lastY - ypos) * sensitivity;
        lastX = xpos;
        lastY = ypos;
        yaw += xoffset;
        pitch += yoffset;
        cameraFront.x = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
        cameraFront.y = std::sin(glm::radians(pitch));
        cameraFront.z = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));
        cameraFront = glm::normalize(cameraFront);

        // W - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos += cameraFront * cameraSpeed;
            #ifndef _NDEBUG
                std::clog << "LOG: (" << __PRETTY_FUNCTION__ 
                          << ") pressed GLFW_KEY_W." << std::endl;
            #endif
        }
        // S - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos -= cameraSpeed * cameraFront;
            #ifndef _NDEBUG
                std::clog << "LOG: (" << __PRETTY_FUNCTION__ 
                          << ") pressed GLFW_KEY_S." << std::endl;
            #endif
        }
        // A - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            #ifndef _NDEBUG
                std::clog << "LOG: (" << __PRETTY_FUNCTION__ 
                          << ") pressed GLFW_KEY_S." << std::endl;
            #endif
        }
        // D - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            #ifndef _NDEBUG
                std::clog << "LOG: (" << __PRETTY_FUNCTION__ 
                          << ") pressed GLFW_KEY_D." << std::endl;
            #endif
        }

        /*radius = 10.0f;
        camX = std::sin(time) * radius;
        camZ = std::cos(time) * radius;
        view = glm::lookAt(
            glm::vec3(camX, 0.0f, camZ),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );*/
        view = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );
        projection = glm::perspective(
            45.0f,
            static_cast<GLfloat>(window.width) / window.height,
            0.1f, 100.0f
        );

        shader.use();
        glUniformMatrix4fv(
            viewLoc,
            1, GL_FALSE, glm::value_ptr(view)
        );
        glUniformMatrix4fv(
            projectionLoc,
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
        
        for (GLuint i = 0; i < 10; ++i) {
            glBindVertexArray(VAO);
            model = glm::translate(glm::mat4(), cubePos[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(
                modelLoc,
                1, GL_FALSE, glm::value_ptr(model)
            );
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
    }

protected:
    GLuint texture[2]{0}, VBO{0}, VAO{0};
    mylib::Shader shader;
    glm::mat4 trans;
    glm::mat4 model{0}, view{0}, projection{0};
    GLint modelLoc{0}, viewLoc{0}, projectionLoc{0};

    GLfloat radius{0}, camX{0}, camZ{0}, cameraSpeed{0.05f};
    glm::vec3 cameraPos, cameraFront, cameraUp;

    GLfloat deltaTime{0}, lastFrameTime{0};

    GLfloat xoffset{0}, yoffset{0}, sensitivity{0}, yaw{0}, pitch{0};
    double lastX{0}, lastY{0}, xpos{0}, ypos{0};

private:

};

int main(const int argc, const char* const argv[])
{
    mylib::Window wind;
    myApp app(std::move(wind));
    app.run(app);

    std::exit(EXIT_SUCCESS);
}
