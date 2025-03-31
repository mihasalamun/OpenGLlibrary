#ifndef VBO_IPP
#define VBO_IPP

// Constructor that generates a Vertex Buffer Object and links it to vertices
template <typename VertexType>
VBO<VertexType>::VBO(std::vector<VertexType> &vertices)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), vertices.data(), GL_STATIC_DRAW);
}

// Binds the VBO
template <typename VertexType>
void VBO<VertexType>::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

// Unbinds the VBO
template <typename VertexType>
void VBO<VertexType>::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
template <typename VertexType>
void VBO<VertexType>::Delete()
{
    glDeleteBuffers(1, &m_ID);
}
#endif