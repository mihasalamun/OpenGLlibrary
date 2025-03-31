#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : m_FBO(),
                                                  // m_RBO(width, height),
                                                  m_texture(TextureData("", framebuffer, GL_RGB))
{
    m_texture.Bind();
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;
}

Framebuffer::~Framebuffer()
{
    m_FBO.Delete();
    // m_RBO.Delete();
    m_texture.Delete();
}

void Framebuffer::Bind()
{
    m_FBO.Bind();
}

void Framebuffer::Unbind()
{
    m_FBO.Unbind();
}

void Framebuffer::texUnit(std::shared_ptr<Shader> shader, const std::string uniform, GLuint unit)
{
    m_texture.texUnit(shader, uniform, unit);
    m_texture.Bind();
}
