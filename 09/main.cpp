#undef _NDEBUG

#include "../lib/mylib.hpp"
#include "../lib/camera.hpp"

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
#include <cstdio>
#include <cstdlib>

/*
*   ## Phong Lighting Model

    - Ambient lighting: a distant light) so objects are almost never completely dark.
        To simulate this we use an ambient lighting constant that always 
        gives the object some color.
    
    - Diffuse lighting: simulates the directional impact a light object has on an object.
        The more a part of an object faces the light source, the brighter it becomes.
    
    - Specular lighting: simulates the bright spot of a light that appears on shiny objects.
        Specular highlights are often more inclined to the color of the light 
        than the color of the object.

*/

GLfloat vertices_box[] = {
    0.250000f, -0.250000f, -0.250000f,  // 0,
    0.250000f, -0.250000f, 0.250000f,   // 1
    -0.250000f, -0.250000f, 0.250000f,  // 2
    -0.250000f, -0.250000f, -0.250000f, // 3
    0.250000f, 0.250000f, -0.250000f,   // 4
    0.250000f, 0.250000f, 0.250000f,    // 5
    -0.250000f, 0.250000f, 0.250000f,   // 6
    -0.250000f, 0.250000f, -0.250000f,  // 7
};
GLfloat vertices_lightbox[24];

GLuint indices[] = {
    0, 1, 2, // bottom face
    0, 2, 3,
    0, 1, 5, // right face
    0, 5, 4,
    0, 4, 3, // back face
    4, 7, 3,
    3, 2, 6, // left face
    3, 6, 7,
    1, 2, 6, // front face
    6, 5, 1,
    6, 7, 5, // top face
    7, 4, 5,
};

class myApp : public mylib::App
{
  public:
    myApp(mylib::Window &&window) : mylib::App(std::move(window)) {}

    void startup() override
    {
        lightingShader = mylib::Shader({
            std::make_pair(
                "/home/sam/workspace/cpp/ogl_playground/07/07.vertex.glsl",
                GL_VERTEX_SHADER),
            std::make_pair(
                //"/home/sam/workspace/cpp/ogl_playground/07/07.fragment.glsl",
                "/home/sam/workspace/cpp/ogl_playground/09/ambientLighting.fragment.glsl",
                GL_FRAGMENT_SHADER),
        });
        lightSrcShader = mylib::Shader({
            std::make_pair(
                "/home/sam/workspace/cpp/ogl_playground/07/07.vertex.glsl",
                GL_VERTEX_SHADER),
            std::make_pair(
                //"/home/sam/workspace/cpp/ogl_playground/07/07.fragment.glsl",
                "/home/sam/workspace/cpp/ogl_playground/09/lightSrc.fragment.glsl",
                GL_FRAGMENT_SHADER),
        });

        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);
        glGenBuffers(1, EBO);
        boxVAO = VAO[0];

        lightPos = glm::vec3(1.f, 1.f, -1.5f);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // box
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_box), vertices_box, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(boxVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glVertexAttribPointer(
            0, 3,
            GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
            (GLvoid *)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        objectColorLoc = glGetUniformLocation(lightingShader.program, "objectColor");
        lightColorLoc = glGetUniformLocation(lightingShader.program, "lightColor");

        camera = mylib::FPSCamera(
            glm::vec3(0.0f, 0.0f, 3.0f), // cameraPos
            glm::vec3(0.0f, 1.0f, 0.0f));
        cameraController = mylib::CameraController(
            &camera, // worldUp
            &window);

        glEnable(GL_DEPTH_TEST);
    }

    ~myApp()
    {
    }

    void update(double time) override
    {
        cameraController.update(cameraController, time);

        view = cameraController.camera->getViewMatrix();
        projection = glm::perspective(
            45.0f,
            static_cast<GLfloat>(window.width) / window.height,
            0.1f, 100.0f);

        lightingShader.use();
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(
            glGetUniformLocation(lightingShader.program, "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(
            glGetUniformLocation(lightingShader.program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));

        lightSrcShader.use();
        glUniform3f(
            glGetUniformLocation(lightSrcShader.program, "lightColor"),
            1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(
            glGetUniformLocation(lightSrcShader.program, "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(
            glGetUniformLocation(lightSrcShader.program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
    }

    void render() override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
        glUniformMatrix4fv(
            glGetUniformLocation(lightingShader.program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(boxVAO);
        glDrawElements(GL_TRIANGLES, 32, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        lightSrcShader.use();
        model = glm::translate(glm::mat4(), lightPos);
        model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(
            glGetUniformLocation(lightSrcShader.program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(boxVAO);
        glDrawElements(GL_TRIANGLES, 32, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

  protected:
    GLuint VAO[5]{0}, VBO[5]{0}, EBO[5]{0};
    mylib::Shader lightingShader, lightSrcShader;

    glm::vec3 lightPos;

    GLuint boxVAO{0};
    GLint objectColorLoc{0}, lightColorLoc{0};

    mylib::Camera camera;
    mylib::CameraController cameraController;

    glm::mat4 model{0}, view{0}, projection{0};

  private:
};

int main(const int argc, const char *const argv[])
{
    mylib::Window wind;
    myApp app(std::move(wind));
    app.run(app);

    std::exit(EXIT_SUCCESS);
}
