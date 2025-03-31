#include "RBO.h"

RBO::RBO(int width, int height)
{
    glCreateRenderbuffers(1, &m_ID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
    glRenderbufferStorage(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ID);
}

void RBO::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

void RBO::Unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RBO::Delete()
{
    glDeleteRenderbuffers(1, &m_ID);
}
