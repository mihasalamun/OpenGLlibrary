#ifndef RBO_H
#define RBO_H
#include <glad/gl.h>

class RBO
{
private:
    GLuint m_ID;

public:
    RBO(int width, int height);

    void Bind();

    void Unbind();

    void Delete();
};

#endif