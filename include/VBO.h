#ifndef VBO_H
#define VBO_H

#include <glm/glm.hpp>
#include <glad/gl.h>
#include <vector>
#include <optional>

// Structure to standardize the vertices used in the meshes
struct Vertex3D
{
    std::optional<glm::vec3> position;
    std::optional<glm::vec3> normal;
    std::optional<glm::vec3> color;
    std::optional<glm::vec2> texUV;
};

struct Vertex2D
{
    std::optional<glm::vec2> position;
    std::optional<glm::vec3> color;
    std::optional<glm::vec2> texUV;
};

template <typename VertexType>
class VBO
{
private:
    // Reference ID of the Vertex Buffer Object
    GLuint m_ID;

public:
    // Constructor that generates a Vertex Buffer Object and links it to vertices
    VBO(std::vector<VertexType> &vertices);

    // Binds the VBO
    void Bind();
    // Unbinds the VBO
    void Unbind();
    // Deletes the VBO
    void Delete();
};

#include "VBO.ipp"
#endif