#ifndef MESH_IPP
#define MESH_IPP
#include "Mesh.h"

template <typename VertexType>
Mesh<VertexType>::Mesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices)
    : m_vertices(vertices), m_indices(indices), m_textures(0)
{
    setMesh();
}

template <typename VertexType>
Mesh<VertexType>::Mesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices, std::vector<std::shared_ptr<Texture>> textures)
    : m_vertices(vertices), m_indices(indices), m_textures(textures)
{
    setMesh();
}

template <typename VertexType>
inline Mesh<VertexType>::Mesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices, std::vector<std::string> texNames)
    : m_vertices(vertices), m_indices(indices), m_textures(texNames.size())
{
    for (size_t i = 0; i < texNames.size(); i++)
    {
        m_textures[i] = ResourceManager::GetTexture(texNames[i]);
    }
    setMesh();
}

template <typename VertexType>
Mesh<VertexType>::Mesh()
{
}

template <typename VertexType>
inline Mesh<VertexType>::~Mesh()
{
    m_VAO.Delete();
}

template <typename VertexType>
void Mesh<VertexType>::setMesh()
{
    m_VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO(m_vertices);
    // Generates Element Buffer Object and links it to indices
    EBO EBO(m_indices);
    // Links VBO attributes such as coordinates and colors to VAO

    int i = 0;
    if constexpr (std::is_same<VertexType, Vertex3D>::value)
    {
        if (m_vertices[0].position)
            m_VAO.LinkAttrib(VBO, i++, 3, GL_FLOAT, sizeof(Vertex3D), (void *)0);
        if (m_vertices[0].normal)
            m_VAO.LinkAttrib(VBO, i++, 3, GL_FLOAT, sizeof(Vertex3D), (void *)(4 * sizeof(float)));
        if (m_vertices[0].color)
            m_VAO.LinkAttrib(VBO, i++, 3, GL_FLOAT, sizeof(Vertex3D), (void *)(8 * sizeof(float)));
        if (m_vertices[0].texUV)
            m_VAO.LinkAttrib(VBO, i++, 2, GL_FLOAT, sizeof(Vertex3D), (void *)(12 * sizeof(float)));
    }
    else if (std::is_same<VertexType, Vertex2D>::value)
    {
        if (m_vertices[0].position)
            m_VAO.LinkAttrib(VBO, i++, 2, GL_FLOAT, sizeof(Vertex2D), (void *)0);
        if (m_vertices[0].color)
            m_VAO.LinkAttrib(VBO, i++, 3, GL_FLOAT, sizeof(Vertex2D), (void *)(3 * sizeof(float)));
        if (m_vertices[0].texUV)
            m_VAO.LinkAttrib(VBO, i++, 2, GL_FLOAT, sizeof(Vertex2D), (void *)(7 * sizeof(float)));
    }
    else
    {
        throw "Wrong VertexType in Mesh";
        exit(1);
    }

    // Unbind all to prevent accidentally modifying them
    m_VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    VBO.Delete();
    EBO.Delete();
}

template <typename VertexType>
inline void Mesh<VertexType>::setMesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices) // ? Mogoče daj to bek pa naredi texfiledata na neki default
{
    m_vertices = vertices;
    m_indices = indices;
    setMesh();
}

template <typename VertexType>
void Mesh<VertexType>::setMesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices, std::vector<std::shared_ptr<Texture>> textures)
{
    m_vertices = vertices;
    m_indices = indices;
    m_textures = textures;
    setMesh();
}

template <typename VertexType>
void Mesh<VertexType>::setMesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices, std::vector<std::string> texNames)
{
    m_vertices = vertices;
    m_indices = indices;

    for (size_t i = 0; i < texNames.size(); i++)
    {
        m_textures.push_back(ResourceManager::GetTexture(texNames[i])); // TODO: mogoče ni push_back glih fajn no
    }
    setMesh();
}

template <typename VertexType>
template <typename CameraType>
void Mesh<VertexType>::Draw(std::shared_ptr<Shader> shader, CameraType &camera)
{
    // Bind shader to be able to access uniforms
    shader->Activate();
    m_VAO.Bind();

    if (m_textures.size() != 0) // TODO: tota cela stvar ni glih v redu urejena
    {
        if constexpr (std::is_same<CameraType, Camera3D>::value)
        {
            // Keep track of how many of each type of textures we have
            unsigned int numDiffuse = 0;
            unsigned int numSpecular = 0;

            for (unsigned int i = 0; i < m_textures.size(); i++)
            {
                std::string num;
                TextureType type = m_textures[i]->getType();
                if (type == diffuse)
                {
                    num = std::to_string(numDiffuse++);
                }
                else if (type == specular)
                {
                    num = std::to_string(numSpecular++);
                }
                m_textures[i]->texUnit(shader, (TexTypeToString[type] + num).c_str(), i);
                m_textures[i]->Bind();
            }
        }
        else if constexpr (std::is_same<CameraType, Camera2D>::value)
        {
            // TODO: dodaj framebuffer aj ges
            for (size_t i = 0; i < m_textures.size(); i++)
            {
                m_textures[i]->texUnit(shader, "tex" + std::to_string(i), i + 1);
                m_textures[i]->Bind();
            }
        }
    }

    camera.Matrix(shader, "camMatrix");

    // Draw the actual mesh
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0); // TODO: naredi GL_TRIANGLE_STRIP
}

#endif