#ifndef VAO_IPP
#define VAO_IPP
// Constructor that generates a VAO ID
template <typename VertexType>
VAO<VertexType>::VAO()
{
    glGenVertexArrays(1, &m_ID);
}

// Links a VBO Attribute such as a position or color to the VAO
template <typename VertexType>
void VAO<VertexType>::LinkAttrib(VBO<VertexType> &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset)
{
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

// Binds the VAO
template <typename VertexType>
void VAO<VertexType>::Bind()
{
    glBindVertexArray(m_ID);
}

// Unbinds the VAO
template <typename VertexType>
void VAO<VertexType>::Unbind()
{
    glBindVertexArray(0);
}

// Deletes the VAO
template <typename VertexType>
void VAO<VertexType>::Delete()
{
    glDeleteVertexArrays(1, &m_ID);
}
#endif