#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "FBO.h"
#include "RBO.h"
#include "VAO.h"
#include "Texture.h"

class Framebuffer
{
private:
    FBO m_FBO;
    // RBO m_RBO;
    Texture m_texture;

public:
    Framebuffer(int width, int height);
    ~Framebuffer();

    // Before window clear
    void Bind();
    void Unbind();
    void texUnit(std::shared_ptr<Shader> shader, const std::string uniform, GLuint unit);
};

#endif