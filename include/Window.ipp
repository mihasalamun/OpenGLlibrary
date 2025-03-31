#ifndef WINDOW_IPP
#define WINDOW_IPP
#include "Window.h"

template <typename Camera>
std::vector<Camera2D> Window<Camera>::cameras2D;

template <typename Camera>
std::vector<Camera3D> Window<Camera>::cameras3D;

template <typename Camera>
Window<Camera>::Window(int width, int height, uint options, uint antialiasing, glm::vec3 clearColor) : m_width(width), m_height(height), m_clearColor(clearColor, 1), m_options(options)
{
    assert(restart_gl_log());
    // start GL context and O/S window using the GLFW helper library
    gl_log("starting GLFW\n%s\n", glfwGetVersionString());
    // register the error call-back function that we wrote, above
    glfwSetErrorCallback(glfw_error_callback);
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Initiation failed" << std::endl;
        exit(1);
    }

    // Tell GLFW what version of OpenGL we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Tell GLFW we are using the CORE profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Anti-Aliasing
    if (antialiasing)
        glfwWindowHint(GLFW_SAMPLES, antialiasing);

    m_window = glfwCreateWindow(width, height, "OpenGL", (m_options & WindowOptions::Fullscreen) ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    // Introduce the window into the current context
    glfwMakeContextCurrent(m_window);
    // Load GLAD so it configures OpenGL
    gladLoadGL(glfwGetProcAddress);
    // Specify the viewport of OpenGL in the Window
    glViewport(0, 0, width, height);

    // Enables the Depth Buffer
    if (m_options & WindowOptions::DepthTest)
    {
        glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_GREATER);
    }

    // Enables the Stencil Buffer
    if (m_options & WindowOptions::StensilTest)
    {
        glEnable(GL_STENCIL_TEST);
        // Sets rules for outcomes of stecil tests
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

    if (antialiasing)
        glEnable(GL_MULTISAMPLE);

    // Enables Cull Facing
    if (m_options & WindowOptions::FaceCulling)
    {
        glEnable(GL_CULL_FACE);
        // Keeps front faces
        // glCullFace(GL_FRONT);
        // Uses counter clock-wise standard
        // glFrontFace(GL_CCW);
        // glFrontFace(GL_CW);
    }

    if (m_options & WindowOptions::Blend)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    if (m_options & WindowOptions::CubemapSeamless)
    {
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetWindowSizeCallback(m_window, WindowSizeCallback);
    glfwSetScrollCallback(m_window, ScrollCallback);
}

template <typename Camera>
inline Window<Camera>::Window(int width, int height, glm::vec2 position, double zoom, uint options, uint antialiasing, glm::vec3 clearColor)
    : Window(width, height, options, antialiasing, clearColor)
{
    static_assert(std::is_same<Camera, Camera2D>::value, "Used Camera2D constructor with wrong template type!");
    cameras2D.push_back(Camera2D(m_window, m_width, m_height, position, zoom));
}

template <typename Camera>
inline Window<Camera>::Window(int width, int height, glm::vec3 orientation, float distance, float FOVdeg, float nearPlane, float farPlane, uint options, uint antialiasing, glm::vec3 clearColor)
    : Window(width, height, options, antialiasing, clearColor)
{
    static_assert(std::is_same<Camera, Camera3D>::value, "Used Camera3D constructor with wrong template type!");
    cameras3D.push_back(Camera3D(m_window, m_width, m_height, FOVdeg, nearPlane, farPlane, orientation, distance));
}

template <typename Camera>
Window<Camera>::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

template <typename Camera>
int Window<Camera>::ShouldClose()
{
    return glfwWindowShouldClose(m_window);
}

template <typename Camera>
void Window<Camera>::Clear()
{
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    uint clearOptions = GL_COLOR_BUFFER_BIT;
    if (m_options & WindowOptions::DepthTest)
        clearOptions |= GL_DEPTH_BUFFER_BIT;
    if (m_options & WindowOptions::StensilTest)
        clearOptions |= GL_STENCIL_BUFFER_BIT;
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // glClear(GL_COLOR_BUFFER_BIT);
    glClear(clearOptions);
}

template <typename Camera>
void Window<Camera>::SwapBuffers()
{
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(m_window);
    // Take care of all GLFW events
    glfwPollEvents();
}

template <typename Camera>
inline void Window<Camera>::Inputs()
{
    GetMainCamera().Inputs(m_window);
}

template <typename Camera>
inline void Window<Camera>::UpdateMatrix()
{
    GetMainCamera().UpdateMatrix();
}

template <typename Camera>
inline Camera &Window<Camera>::GetMainCamera()
{
    if constexpr (std::is_same<Camera, Camera2D>::value)
        return GetMainCamera2D();
    else if constexpr (std::is_same<Camera, Camera3D>::value)
        return GetMainCamera3D();
}

template <typename Camera>
void Window<Camera>::WindowSizeCallback(GLFWwindow *window, int width, int height)
{
    Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->WindowSizeCallback(width, height);
}

template <typename Camera>
void Window<Camera>::WindowSizeCallback(int width, int height)
{
    glViewport(0, 0, width, height);

    for (auto &camera : cameras2D)
    {
        camera.SetSize(width, height);
    }

    for (auto &camera : cameras3D)
    {
        camera.SetSize(width, height);
    }
}

template <typename Camera>
void Window<Camera>::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->ScrollCallback(xoffset, yoffset);
}

template <typename Camera>
void Window<Camera>::ScrollCallback(double xoffset, double yoffset)
{
    for (Camera2D &camera : cameras2D)
        camera.ScrollCallback(xoffset, yoffset);

    for (Camera3D &camera : cameras3D)
        camera.ScrollCallback(xoffset, yoffset);
}

#endif
