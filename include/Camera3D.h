#ifndef CAMERA3D_H
#define CAMERA3D_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"
#include <memory>

class Camera3D
{
private:
    // Stores the main vectors of the camera
    glm::vec3 m_up = glm::vec3(0, 1, 0);
    glm::mat4 m_cameraMatrix = glm::mat4(1);

    // Prevents the camera from jumping around when first clicking left click
    bool m_firstClick = true;

    // Stores the width and height of the window
    int m_width;
    int m_height;

    // Adjust the speed of the camera and it's sensitivity when looking around
    double m_speed = 0.1f;
    double m_sensitivity = 100.0f;

    float m_FOVdeg;
    float m_nearPlane;
    float m_farPlane;

    float m_distance;
    glm::vec3 m_orientation;
    glm::vec3 m_position;

public:
    // Camera constructor to set up initial values
    Camera3D(GLFWwindow *window, int width, int height, float FOVdeg = 45, float nearPlane = 0.1, float farPlane = 100, glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f), float distance = 6.0f);
    // Camera3D(Window &window, float FOVdeg, float nearPlane, float farPlane, glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f), float distance = 6.0f);

    // Updates the camera matrix to the Vertex Shader
    void UpdateMatrix();
    // Exports the camera matrix to a shader
    void Matrix(std::shared_ptr<Shader> shader, const char *uniform);
    // Handles camera inputs
    void Inputs(GLFWwindow *window);

    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }

    float getFOV() { return m_FOVdeg; }
    float getNearPlane() { return m_nearPlane; }
    float getFarPlane() { return m_farPlane; }

    glm::vec3 getUp() { return m_up; }
    glm::vec3 getOrientation() { return m_orientation; }
    glm::vec3 GetPosition() { return m_position; }

    void SetSize(int width, int height);

    void ScrollCallback(double xoffset, double yoffset);
};
#endif