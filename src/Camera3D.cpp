#include "Camera3D.h"
#include <format>

Camera3D::Camera3D(GLFWwindow *window, int width, int height, float FOVdeg,
                   float nearPlane, float farPlane, glm::vec3 orientation, float distance)
    : m_width(width), m_height(height), m_FOVdeg(FOVdeg), m_nearPlane(nearPlane), m_farPlane(farPlane),
      m_orientation(glm::normalize(orientation)), m_distance(distance), m_position(-distance * orientation)
{
}

// Camera3D::Camera3D(Window &window, float FOVdeg, float nearPlane, float farPlane, glm::vec3 orientation, float distance)
//     : Camera3D(window.GetGLFWwindow(), window.GetWidth(), window.GetHeight(), FOVdeg, nearPlane, farPlane, orientation, distance)
// {
// }

void Camera3D::UpdateMatrix()
{
    // Initializes matrices since otherwise they will be the null matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    m_position = -m_distance * m_orientation;
    view = glm::lookAt(m_position, m_position + m_orientation, m_up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(m_FOVdeg), (float)m_width / m_height, m_nearPlane, m_farPlane);
    // projection = glm::ortho(-600.0f, 600.0f, -600.0f, 600.0f, m_nearPlane, m_farPlane);

    // Sets new camera matrix
    m_cameraMatrix = projection * view;
}

void Camera3D::Matrix(std::shared_ptr<Shader> shader, const char *uniform)
{
    // Exports camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), uniform), 1, GL_FALSE, glm::value_ptr(m_cameraMatrix));
}

void Camera3D::Inputs(GLFWwindow *window)
{
    // TODO: Naredi Inputs class al nekaj, da je customizable
    // Handles key inputs
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Prevents camera from jumping on the first click
        if (m_firstClick)
        {
            glfwSetCursorPos(window, (m_width / 2), (m_height / 2));
            m_firstClick = false;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float rotX = glm::radians(m_sensitivity * (float)(mouseX - (m_width / 2)) / m_width);
        float rotY = glm::radians(m_sensitivity * (float)(mouseY - (m_height / 2)) / m_height);

        float vertOver = std::abs(glm::angle(m_orientation, m_up) + rotY - M_PI_2) - M_PI_2;

        // Decides whether or not the next vertical orientation is legal or not
        if (vertOver >= 0)
        {

            m_orientation[1] = round(m_orientation[1]);
            float rescale = 1e-5 / std::abs(m_orientation[0]);
            m_orientation[0] = copysign(1e-5, m_orientation[0]);
            m_orientation[2] = m_orientation[2] * rescale;
            m_orientation = glm::normalize(m_orientation);
        }
        else // Calculates upcoming vertical change in the Orientation
            m_orientation = glm::rotate(m_orientation, -(float)rotY, glm::normalize(glm::cross(m_orientation, m_up)));

        // Rotates the Orientation left and right
        m_orientation = glm::rotate(m_orientation, -rotX, m_up);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (m_width / 2), (m_height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        m_firstClick = true;
    }
}

void Camera3D::SetSize(int width, int height)
{
    m_width = width;
    m_height = height;
    UpdateMatrix();
}

void Camera3D::ScrollCallback(double xoffset, double yoffset)
{
    m_distance -= m_distance * yoffset * 0.1;
}