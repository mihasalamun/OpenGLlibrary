#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <eigen3/Eigen/Dense>

#include "Shader.h"
#include <memory>

class Camera2D
{
private:
    // Stores the main vectors of the camera
    glm::vec3 m_position;
    double m_zoom;
    glm::mat4 m_cameraMatrix = glm::mat4(1.0f);

    // Prevents the camera from jumping around when first clicking left click
    bool m_firstClick = true;

    // Stores the width and height of the window
    int m_width;
    int m_height;

    double m_speed = 0.01;
    double m_zoomSpeed = 0.05;

    double m_mouseXP;
    double m_mouseYP;

public:
    // Camera constructor to set up initial values
    Camera2D(GLFWwindow *window, int width, int height, glm::vec2 position, double zoom);
    // Camera2D(Window &window, glm::vec2 position, double zoom, bool setScrollCallback = false);

    // Updates the camera matrix to the Vertex Shader
    void UpdateMatrix();
    // Exports the camera matrix to a shader
    void Matrix(std::shared_ptr<Shader> shader, const char *uniform);
    // Handles camera inputs
    void Inputs(GLFWwindow *window);

    void Move(glm::vec2 move);
    glm::vec2 GetPosition();
    void SetSize(int width, int height);

    Eigen::Vector2d ScreenToWorld(Eigen::Vector2d screen);

    void ScrollCallback(double xoffset, double yoffset);
};
#endif