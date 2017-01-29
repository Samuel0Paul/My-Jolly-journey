#ifndef __MYLIB_CAMERA_HPP__
#define __MYLIB_CAMERA_HPP__

#include <vector>
#include <cmath>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <glm/gtc/matrix_transform.hpp>

namespace mylib
{

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;

// abstract Camera class
class Camera
{
  public:
    glm::vec3 position, front, up, right, worldUp;
    GLfloat yaw{0}, pitch{0}, movementSpeed{0}, mouseSensitivity{0}, zoom{0};

    // with vectors
    Camera(
        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
        glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
        GLfloat yaw = YAW,
        GLfloat pitch = PITCH) : front(glm::vec3(0.f, 0.f, -1.0f)),
                                 movementSpeed(SPEED),
                                 mouseSensitivity(SENSITIVITY),
                                 zoom(ZOOM)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }

    // with scalars
    Camera(
        GLfloat posX,
        GLfloat posY,
        GLfloat posZ,
        GLfloat upX,
        GLfloat upY,
        GLfloat upZ,
        GLfloat yaw,
        GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
                         movementSpeed(SPEED),
                         mouseSensitivity(SENSITIVITY),
                         zoom(ZOOM)
    {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(
            this->position,
            this->position + this->front,
            this->up);
    }

    // Processes input received from any keyboard-like input system.
    // Accepts input parameter in the form of camera defined ENUM
    //  (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->movementSpeed * deltaTime;
        if (direction == FORWARD)
            this->position += this->front * velocity;
        if (direction == BACKWARD)
            this->position -= this->front * velocity;
        if (direction == LEFT)
            this->position -= this->right * velocity;
        if (direction == RIGHT)
            this->position += this->right * velocity;
    }

    // Processes input received from a mouse input system.
    // Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->mouseSensitivity;
        yoffset *= this->mouseSensitivity;

        this->yaw += xoffset;
        this->pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->pitch > 89.0f)
                this->pitch = 89.0f;
            if (this->pitch < -89.0f)
                this->pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset)
    {
        if (this->zoom >= 1.0f && this->zoom <= 45.0f)
            this->zoom -= yoffset;
        if (this->zoom <= 1.0f)
            this->zoom = 1.0f;
        if (this->zoom >= 45.0f)
            this->zoom = 45.0f;
    }

  private:
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
        front.y = std::sin(glm::radians(this->pitch));
        front.z = std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));

        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
};

} // namespace mylib

#endif
