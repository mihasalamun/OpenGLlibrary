#ifndef SURFACE_H
#define SURFACE_H

#include "Mesh.h"
#include "util.h"

class Surface3D
{
private:
    glm::mat4 model;
    Mesh<Vertex3D> mesh;

public:
    Surface3D(std::vector<std::vector<glm::vec3>> points, std::optional<std::vector<std::vector<glm::vec3>>> normals = std::nullopt, glm::vec3 center = glm::vec3(0.0f),
              std::optional<glm::vec3> color = std::nullopt, bool firstPeriodic = false, bool secondPeriodic = false);

    Surface3D(std::vector<Vertex3D> &vertices, std::vector<GLuint> &indicies, glm::vec3 center);

    Surface3D();

    void setMesh(std::vector<std::vector<glm::vec3>> points, std::optional<std::vector<std::vector<glm::vec3>>> normals = std::nullopt,
                 std::optional<glm::vec3> color = std::nullopt, bool firstPeriodic = false, bool secondPeriodic = false);

    void Draw(std::shared_ptr<Shader> shader, Camera3D &camera);
    void Draw(std::shared_ptr<Shader> shader, Camera3D &camera, glm::vec3 color);
    void Draw(std::shared_ptr<Shader> shader, Camera3D &camera, glm::vec4 color);
    void DrawWireframe(Camera3D &camera, float scale = 1.0f, glm::vec3 color = glm::vec3(0, 0, 0));

    void Rotate(float angle, glm::vec3 axis);
    void Rotate(float angle, glm::vec3 axis, glm::vec3 center);
    void Scale(float scale);
    void Scale(glm::vec3 scale);
    void Translate(glm::vec3 v);

    void setPosition(glm::vec3 pos);
    void setPosition(Eigen::Vector3d pos);
    void setPositionRotation(glm::vec3 pos, glm::vec3 axis);
    void setPositionRotation(Eigen::Vector3d pos, Eigen::Vector3d axis);
};

Surface3D Ellipsoid(float a, float b, float c, glm::vec3 center, std::optional<glm::vec3> color = std::nullopt, int num = 100, bool setNormals = false);

Surface3D Sphere(float r, glm::vec3 center, std::optional<glm::vec3> color = std::nullopt, int num = 100, bool setNormals = false);

Surface3D Rectangle(glm::vec3 normal, float rotation, glm::vec3 center, float a, float b, std::optional<glm::vec3> color = std::nullopt, bool setNormals = false);

Surface3D RectCuboid(glm::vec3 center, float a, float b, float c, std::optional<glm::vec3> color = std::nullopt);

#endif