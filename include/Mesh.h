#ifndef MESH_H
#define MESH_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera3D.h"
#include "Camera2D.h"
#include "Texture.h"
#include "ResourceManager.h"

extern std::vector<Vertex3D> cubeVertices;
extern std::vector<GLuint> cubeIndices;

template <typename VertexType>
class Mesh
{
private:
    std::vector<VertexType> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<std::shared_ptr<Texture>> m_textures;
    // Store VAO in public so it can be used in the Draw function
    VAO<VertexType> m_VAO;

public:
    // Initializes the mesh
    Mesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices);
    Mesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices, std::vector<std::shared_ptr<Texture>> textures);
    Mesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices, std::vector<std::string> texNames);
    Mesh();
    ~Mesh();

    void setMesh();
    void setMesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices);
    void setMesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices, std::vector<std::shared_ptr<Texture>> textures);
    void setMesh(std::vector<VertexType> &vertices, std::vector<GLuint> &indices, std::vector<std::string> texNames);
    // Draws the mesh
    template <typename CameraType>
    void Draw(std::shared_ptr<Shader> shader, CameraType &camera);
};

#include "Mesh.ipp"
#endif
