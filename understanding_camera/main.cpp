/*
*   just a playground to understand my camera hehehe
*/

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

#undef _NDEBUG

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

class myAppUsingCameraClass : public mylib::App
{
  public:
    myAppUsingCameraClass(mylib::Window &&window) : mylib::App(std::move(window))
    {
    }

    void startup() override
    {
        lightingShader = mylib::Shader({
            std::make_pair(
                "/home/sam/workspace/cpp/ogl_playground/07/07.vertex.glsl",
                GL_VERTEX_SHADER),
            std::make_pair(
                "/home/sam/workspace/cpp/ogl_playground/07/07.fragment.glsl",
                GL_FRAGMENT_SHADER),
        });

        // 1 - box, 2 - lampBox
        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);
        glGenBuffers(1, EBO);
        boxVAO = VAO[0];

        lightPos = glm::vec3(4.f, 4.f, 2.f);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // box
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_box), vertices_box, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // obj VAO
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
            glm::vec3(0.0f, 0.0f, 3.0f),  // cameraPos
            glm::vec3(0.0f, 1.0f, 0.0f)); // worldUp

        // mouse setup
        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window.getWindow(), window.width, window.height);
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        lastX = static_cast<GLfloat>(xpos);
        lastY = static_cast<GLfloat>(ypos);

        glEnable(GL_DEPTH_TEST);
    }

    ~myAppUsingCameraClass()
    {
    }

    void update(double time) override
    {
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        xoffset = static_cast<GLfloat>(xpos - lastX);
        // reversed y cord, botton to top
        yoffset = static_cast<GLfloat>(lastY - ypos);
        lastX = xpos;
        lastY = ypos;
        camera.processMouseMovement(xoffset, yoffset);

        std::cout << xoffset << ", " << yoffset
                  << "\tlast: (" << lastX << ", " << lastY << ")"
                  << "\tpos: (" << xpos << ", " << ypos << ")" << std::endl;

        // W - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.processKeyboard(mylib::FORWARD, deltaTime);
        }
        // S - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.processKeyboard(mylib::BACKWARD, deltaTime);
        }
        // A - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.processKeyboard(mylib::LEFT, deltaTime);
        }
        // D - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.processKeyboard(mylib::RIGHT, deltaTime);
        }

        view = camera.getViewMatrix();
        projection = glm::perspective(
            45.0f,
            static_cast<GLfloat>(window.width) / window.height,
            0.1f, 100.0f);

        lightingShader.use();
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
        glUniformMatrix4fv(
            glGetUniformLocation(lightingShader.program, "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(
            glGetUniformLocation(lightingShader.program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
        //model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(
            glGetUniformLocation(lightingShader.program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
    }

    void render() override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        glBindVertexArray(boxVAO);
        glDrawElements(GL_TRIANGLES, 32, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

  protected:
    GLuint VAO[5]{0}, VBO[5]{0}, EBO[5]{0};
    mylib::Shader lightingShader;

    glm::vec3 lightPos;

    GLuint boxVAO{0};
    GLint objectColorLoc{0}, lightColorLoc{0};

    mylib::FPSCamera camera;

    glm::mat4 model{0}, view{0}, projection{0};

    GLfloat deltaTime{0}, lastFrameTime{0};

    GLfloat xoffset{0}, yoffset{0};
    double lastX{0}, lastY{0}, xpos{0}, ypos{0};

  private:
};

/*
class myApp : public mylib::App
{
  public:
    myApp(mylib::Window &&window) : mylib::App(std::move(window))
    {
    }

    void startup() override
    {
        lightingShader = mylib::Shader({
            std::make_pair(
                "/home/sam/workspace/cpp/ogl_playground/07/07.vertex.glsl",
                GL_VERTEX_SHADER),
            std::make_pair(
                "/home/sam/workspace/cpp/ogl_playground/07/07.fragment.glsl",
                GL_FRAGMENT_SHADER),
        });

        // 1 - box, 2 - lampBox
        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);
        glGenBuffers(1, EBO);
        boxVAO = VAO[0];

        lightPos = glm::vec3(4.f, 4.f, 2.f);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // box
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_box), vertices_box, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // obj VAO
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

        cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        yaw = -90.0f;

        // mouse setup
        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        sensitivity = 0.05f;
        glfwSetCursorPos(window.getWindow(), window.width, window.height);
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        lastX = static_cast<GLfloat>(xpos);
        lastY = static_cast<GLfloat>(ypos);

        glEnable(GL_DEPTH_TEST);
    }

    ~myApp()
    {
    }

    void update(double time) override
    {
        std::cout << "time: " << time << std::endl;
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;
        cameraSpeed = 0.5f * deltaTime;

        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        xoffset = static_cast<GLfloat>(xpos - lastX) * sensitivity;
        // reversed y cord, botton to top
        yoffset = static_cast<GLfloat>(lastY - ypos) * sensitivity;
        lastX = xpos;
        lastY = ypos;
        //yaw += xoffset;
        yaw = std::fmod((yaw + xoffset), (GLfloat)360.0f);
        pitch += yoffset;
        cameraFront.x = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
        cameraFront.y = std::sin(glm::radians(pitch));
        cameraFront.z = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));
        cameraFront = glm::normalize(cameraFront);

        std::cout << xoffset << ", " << yoffset
                  << "\tlast: (" << lastX << ", " << lastY << ")"
                  << "\tpos: (" << xpos << ", " << ypos << ")" << std::endl;

        // W - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos += cameraFront * cameraSpeed;
#ifndef _NDEBUG
            std::clog << "LOG: (" << __PRETTY_FUNCTION__
                      << ") pressed GLFW_KEY_W." << std::endl;
#endif
        }
        // S - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos -= cameraSpeed * cameraFront;
#ifndef _NDEBUG
            std::clog << "LOG: (" << __PRETTY_FUNCTION__
                      << ") pressed GLFW_KEY_S." << std::endl;
#endif
        }
        // A - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
#ifndef _NDEBUG
            std::clog << "LOG: (" << __PRETTY_FUNCTION__
                      << ") pressed GLFW_KEY_S." << std::endl;
#endif
        }
        // D - key press
        if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
#ifndef _NDEBUG
            std::clog << "LOG: (" << __PRETTY_FUNCTION__
                      << ") pressed GLFW_KEY_D." << std::endl;
#endif
        }

        view = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp);
        projection = glm::perspective(
            45.0f,
            static_cast<GLfloat>(window.width) / window.height,
            0.1f, 100.0f);

        lightingShader.use();
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
        glUniformMatrix4fv(
            glGetUniformLocation(lightingShader.program, "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(
            glGetUniformLocation(lightingShader.program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
        //model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(
            glGetUniformLocation(lightingShader.program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
    }

    void render() override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        glBindVertexArray(boxVAO);
        glDrawElements(GL_TRIANGLES, 32, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

  protected:
    GLuint VAO[5]{0}, VBO[5]{0}, EBO[5]{0};
    mylib::Shader lightingShader;

    glm::vec3 lightPos;

    GLuint boxVAO{0};
    GLint objectColorLoc{0}, lightColorLoc{0};

    glm::mat4 model{0}, view{0}, projection{0};

    GLfloat radius{0}, camX{0}, camZ{0}, cameraSpeed{0.05f};
    glm::vec3 cameraPos, cameraFront, cameraUp;

    GLfloat deltaTime{0}, lastFrameTime{0};

    GLfloat xoffset{0}, yoffset{0}, sensitivity{0}, yaw{0}, pitch{0};
    double lastX{0}, lastY{0}, xpos{0}, ypos{0};

  private:
};
*/

int main(const int argc, const char *const argv[])
{
    glm::vec3 cameraPos, cameraTarget, cameraDirection, cameraUp,
        cameraRight;
    glm::mat4 view, model;

    cameraPos = glm::vec3(2.0f, 1.0f, 3.0f);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = glm::normalize(glm::cross(
        glm::vec3(0.0f, 1.0f, 0.0f), // vec pointing up in y axis
        cameraDirection));
    //cameraUp        = glm::cross(cameraDirection, cameraRight);

    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    /*
    *   ```
        lookAt = {((Rx, Ry, Rz, 0),      ((1, 0, 0, -Px),   }
                 { (Ux, Uy, Uz, 0),  *    (0, 1, 0, -Py),   }
                 { (Dx, Dy, Dz, 0),       (0, 0, 1, -Pz),   }
                 { (0,  0,  0,  0))       (0, 0, 0,  1))    }
            // R - right vector (cameraRight)
            // D - direction vector (cameraDirection)
            // U - up vector (cameraUp)
        ```
    */
    view = glm::lookAt(
        cameraPos,    // cameraPos
        cameraTarget, // cameraTarget
        cameraUp      // cameraUp
        );

    std::cout << "\ncameraPos: \t" << glm::to_string(cameraPos)
              << "\ncameraTarget: \t" << glm::to_string(cameraTarget)
              << "\ncameraUp: \t" << glm::to_string(cameraUp)
              << "\nview: \t\t" << glm::to_string(view) << std::endl;

    mylib::Window wind;
    myAppUsingCameraClass app(std::move(wind));
    app.run(app);

    std::exit(EXIT_SUCCESS);
}
