#include "Light.h"

extern std::vector<Vertex3D> cubeVertices;
extern std::vector<GLuint> cubeIndices;

Light::Light(LightType type, glm::vec4 lightColor, glm::vec3 lightPos, unsigned int mapWidth, float farPlane, float orthWidth)
    : m_type(type), m_shadowMapWidth(mapWidth), m_shadowMapHeight(mapWidth), m_farPlane(farPlane), m_orthWidth(orthWidth),
      m_shadowMapShader(type == point
                            ? ResourceManager::GetShader("shadowCubeMap")
                            : ResourceManager::GetShader("shadowMap")),
      m_lightShader(ResourceManager::GetShader("basic4")),
      m_lightMesh(Sphere(0.1, lightPos, lightColor)),
      m_lightColor(lightColor),
      m_lightPos(lightPos)
{
    if (type == direct || type == spot) // Framebuffer for Shadow Map
    {
        glGenFramebuffers(1, &m_shadowMapFBO);
        glGenTextures(1, &m_shadowMap);
        glBindTexture(GL_TEXTURE_2D, m_shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowMapWidth, m_shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        // Prevents darkness outside the frustrum
        float clampColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

        glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);
        // Needed since we don't touch the color buffer
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Matrices needed for the light's perspective
        glm::mat4 projection;
        if (type == direct)
            projection = glm::ortho(-orthWidth, orthWidth, -orthWidth, orthWidth, 0.0f, farPlane);
        else if (type == spot)
            projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        m_lightProjection = projection * lightView;

        m_shadowMapShader->Activate();
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "lightProjection"), 1, GL_FALSE, glm::value_ptr(m_lightProjection));
    }
    else if (type == point) // Framebuffer for Cubemap Shadow Map
    {

        glGenFramebuffers(1, &m_shadowMapFBO);

        glGenTextures(1, &m_shadowMap);

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                         m_shadowMapWidth, m_shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Matrices needed for the light's perspective on all faces of the cubemap
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
        glm::mat4 shadowTransforms[] =
            {
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))};
        // Export all matrices to shader
        m_shadowMapShader->Activate();
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));
        glUniform3f(glGetUniformLocation(m_shadowMapShader->getID(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform1f(glGetUniformLocation(m_shadowMapShader->getID(), "farPlane"), farPlane);
    }
}

Light::~Light()
{
    Delete();
}

void Light::Delete()
{
    glDeleteFramebuffers(1, &m_shadowMapFBO);
}

void Light::UpdateMatrix()
{
    if (m_type == direct || m_type == spot)
    {
        // Matrices needed for the light's perspective
        glm::mat4 projection;
        if (m_type == direct)
            projection = glm::ortho(-m_orthWidth, m_orthWidth, -m_orthWidth, m_orthWidth, 0.0f, m_farPlane);
        else if (m_type == spot)
            projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, m_farPlane);
        glm::mat4 lightView = glm::lookAt(m_lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        m_lightProjection = projection * lightView;

        m_shadowMapShader->Activate();
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "lightProjection"), 1, GL_FALSE, glm::value_ptr(m_lightProjection));
    }
    else if (m_type == point)
    {
        // Matrices needed for the light's perspective on all faces of the cubemap
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, m_farPlane);
        glm::mat4 shadowTransforms[] =
            {
                shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
                shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
                shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
                shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
                shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
                shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))};
        // Export all matrices to shader
        m_shadowMapShader->Activate();
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
        glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader->getID(), "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));
        glUniform3f(glGetUniformLocation(m_shadowMapShader->getID(), "lightPos"), m_lightPos.x, m_lightPos.y, m_lightPos.z);
        glUniform1f(glGetUniformLocation(m_shadowMapShader->getID(), "farPlane"), m_farPlane);
    }
}

void Light::calculateShadowMap(std::function<void(std::shared_ptr<Shader>)> render)
{
    glViewport(0, 0, m_shadowMapWidth, m_shadowMapHeight);

    // Shadow Map
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    render(m_shadowMapShader);
    // Switch back to the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::Translate(glm::vec3 move)
{
    m_lightPos += move;
    m_lightMesh.Translate(move);
    UpdateMatrix();
}

void Light::setPosition(glm::vec3 pos)
{
    m_lightPos = pos;
    m_lightMesh.setPosition(pos);
    UpdateMatrix();
}

void Light::Draw(Camera3D &camera) { m_lightMesh.Draw(m_lightShader, camera, m_lightColor); }

void Light::Set(std::shared_ptr<Shader> shader, Camera3D &camera)
{
    shader->Activate();
    glActiveTexture(GL_TEXTURE0 + 2);
    // glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
    // glUniform1i(glGetUniformLocation(shader->getID(), "shadowMap"), 0);
    glUniform1i(glGetUniformLocation(shader->getID(), "shadowCubeMap"), 2);
    glUniform4f(glGetUniformLocation(shader->getID(), "lightColor"), m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w);
    glUniform3f(glGetUniformLocation(shader->getID(), "lightPos"), m_lightPos.x, m_lightPos.y, m_lightPos.z);
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "lightProjection"), 1, GL_FALSE, glm::value_ptr(m_lightProjection));
    glUniform1f(glGetUniformLocation(shader->getID(), "farPlane"), m_farPlane);
    glm::vec3 cameraPosition = camera.GetPosition();
    glUniform3f(glGetUniformLocation(shader->getID(), "camPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);
}