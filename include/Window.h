#ifndef WINDOW_H
#define WINDOW_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "gl_log.h"
#include "glm/glm.hpp"

#include <Camera2D.h>
#include <Camera3D.h>

enum WindowOptions
{
    DepthTest = 0x1,
    FaceCulling = 0x2,
    StensilTest = 0x4,
    Blend = 0x8,
    Fullscreen = 0x10,
    CubemapSeamless = 0x20
};

typedef unsigned int uint; // Windows stvar

template <typename Camera>
class Window
{
private:
    const int m_width, m_height;
    GLFWwindow *m_window;
    const glm::vec4 m_clearColor;
    const uint m_options;

public:
    static std::vector<Camera2D> cameras2D;
    static std::vector<Camera3D> cameras3D;

    Window(int width, int height, uint options = 0, uint antialiasing = 8, glm::vec3 clearColor = glm::vec3(0, 0, 0));
    Window(int width, int height, glm::vec2 position, double zoom, uint options = 0, uint antialiasing = 8, glm::vec3 clearColor = glm::vec3(0, 0, 0));
    Window(int width, int height, glm::vec3 orientation, float distance, float FOVdeg = 90, float nearPlane = 0.1, float farPlane = 100, uint options = 0, uint antialiasing = 8, glm::vec3 clearColor = glm::vec3(0, 0, 0));
    ~Window();

    int ShouldClose();

    void Clear();
    void SwapBuffers();

    void Inputs();
    void UpdateMatrix();

    GLFWwindow *GetGLFWwindow() { return m_window; }
    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }
    Camera &GetMainCamera();

private:
    Camera2D &GetMainCamera2D() { return cameras2D[0]; }
    Camera3D &GetMainCamera3D() { return cameras3D[0]; }

    void static WindowSizeCallback(GLFWwindow *window, int width, int height);
    void WindowSizeCallback(int width, int height);

    void static ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void ScrollCallback(double xoffset, double yoffset);
};

#include "Window.ipp"

#endif