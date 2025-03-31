#include "Camera2D.h"

Camera2D::Camera2D(GLFWwindow *window, int width, int height, glm::vec2 position, double zoom)
    : m_width(width), m_height(height), m_position(glm::vec3(position, 0)), m_zoom(zoom)
{
}

// Camera2D::Camera2D(Window &window, glm::vec2 position, double zoom, bool setScrollCallback)
//     : Camera2D(window.GetGLFWwindow(), window.GetWidth(), window.GetHeight(), position, zoom, setScrollCallback)
// {
// }

void Camera2D::UpdateMatrix()
{
    // Initializes matrices since otherwise they will be the null matrix
    m_cameraMatrix = glm::ortho(-(float)m_width / m_height, (float)m_width / m_height, -1.0f, 1.0f) *
                     glm::scale(glm::vec3(m_zoom, m_zoom, 1)) * glm::translate(m_position);
}

void Camera2D::Matrix(std::shared_ptr<Shader> shader, const char *uniform)
{
    // Exports camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), uniform), 1, GL_FALSE, glm::value_ptr(m_cameraMatrix));
}

void Camera2D::Inputs(GLFWwindow *window)
{
    // TODO: Naredi Inputs class al nekaj, da je customizable
    // Handles key inputs
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    // {
    //     m_position[1] += m_speed / m_zoom;
    // }
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    // {
    //     m_position[1] -= m_speed / m_zoom;
    // }
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    // {
    //     m_position[0] += m_speed / m_zoom;
    // }
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    // {
    //     m_position[0] -= m_speed / m_zoom;
    // }
    // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    // {
    //     m_zoom += m_zoom * m_zoomSpeed;
    // }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        m_zoom -= m_zoom * m_zoomSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_speed = 0.04f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        m_speed = 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {

        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (m_firstClick)
        {
            m_mouseXP = mouseX;
            m_mouseYP = mouseY;
            m_firstClick = false;
        }

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float moveX = (float)(mouseX - m_mouseXP) / m_zoom / m_height * 2;
        float moveY = (float)(mouseY - m_mouseYP) / m_zoom / m_height * 2;

        m_mouseYP = mouseY;
        m_mouseXP = mouseX;

        m_position[0] += moveX;
        m_position[1] -= moveY;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        m_firstClick = true;
    }
}

void Camera2D::Move(glm::vec2 move)
{
    m_position += glm::vec3(-move, 0);
}

glm::vec2 Camera2D::GetPosition()
{
    return {m_position[0], m_position[1]};
}

void Camera2D::SetSize(int width, int height)
{
    m_width = width;
    m_height = height;
    UpdateMatrix();
}

Eigen::Vector2d Camera2D::ScreenToWorld(Eigen::Vector2d screen)
{
    glm::vec3 world = glm::unProject(glm::vec3({screen[0], m_height - screen[1], 0}), glm::scale(glm::vec3(m_zoom, m_zoom, 2)) * glm::translate(m_position),
                                     glm::ortho(-(float)m_width / m_height, (float)m_width / m_height, -1.0f, 1.0f), glm::vec4(0, 0, m_width, m_height));

    return {world[0], world[1]};
}

void Camera2D::ScrollCallback(double xoffset, double yoffset)
{
    m_zoom += m_zoom * yoffset * 0.1;
}