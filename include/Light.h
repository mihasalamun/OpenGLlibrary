#ifndef LIGHT_CLASS_H
#define LIGHT_CLASS_H
#include "Surface3D.h"

#include <functional>

enum LightType
{
    point,
    direct,
    spot
};

class Light
{
private:
    LightType m_type;
    // Texture for Shadow Map FBO
    unsigned int m_shadowMapWidth, m_shadowMapHeight;
    unsigned int m_windowWidth, m_windowHeight;
    // Farplane for perspective
    float m_farPlane, m_orthWidth;
    // Texture for Shadow Map FBO
    GLuint m_shadowMap;

    // Framebuffer for Shadow Map
    GLuint m_shadowMapFBO;

    // Shader program for shadow map
    std::shared_ptr<Shader> m_shadowMapShader;

    // Shader for light cube
    std::shared_ptr<Shader> m_lightShader;
    // Store mesh data in vectors for the mesh
    // Crate light mesh
    Surface3D m_lightMesh;
    glm::vec4 m_lightColor;
    glm::vec3 m_lightPos;
    glm::mat4 m_lightProjection;

public:
    Light(LightType type, glm::vec4 lightColor, glm::vec3 lightPos, unsigned int mapWidth = 2048, float farPlane = 10.0f, float orthWidth = 10.0f);

    ~Light();

    void Delete();

    void UpdateMatrix();
    void calculateShadowMap(std::function<void(std::shared_ptr<Shader>)> render);
    void Translate(glm::vec3 move);
    void setPosition(glm::vec3 pos);

    void Draw(Camera3D &camera);
    void Set(std::shared_ptr<Shader> shader, Camera3D &camera);
};

#endif