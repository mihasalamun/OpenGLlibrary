#ifndef FBO_H
#define FBO_H

#include <glad/gl.h>
#include "Mesh.h"

class FBO
{
private:
    Mesh<Vertex2D> mesh;
    GLuint m_ID;

public:
    FBO();

    void Bind();

    void Unbind();

    void Delete();
};

#endif