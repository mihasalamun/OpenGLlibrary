#include "FBO.h"

std::vector<Vertex2D> rectangleVertices =
    {
        // Coords    // texCoords
        Vertex2D(glm::vec2(1.0f, -1.0f), std::nullopt, glm::vec2(1.0f, 0.0f)),
        Vertex2D(glm::vec2(-1.0f, -1.0f), std::nullopt, glm::vec2(0.0f, 0.0f)),
        Vertex2D(glm::vec2(-1.0f, 1.0f), std::nullopt, glm::vec2(0.0f, 1.0f)),
        Vertex2D(glm::vec2(1.0f, 1.0f), std::nullopt, glm::vec2(1.0f, 1.0f))};

std::vector<GLuint> indicies = {
    0, 1, 2,
    3, 2, 0};

FBO::FBO() : mesh(rectangleVertices, indicies)
{
    glGenFramebuffers(1, &m_ID);
    Bind();
}

void FBO::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void FBO::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Delete()
{
    glDeleteFramebuffers(1, &m_ID);
}
