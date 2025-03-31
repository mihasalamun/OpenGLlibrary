#ifndef SURFACE2D_H
#define SURFACE2D_H

#include "Mesh.h"
#include "util.h"
#include <eigen3/Eigen/Dense>

class Surface2D
{
private:
    Mesh<Vertex2D> mesh;

protected:
    glm::mat4 model;

public:
    Surface2D(std::vector<glm::vec2> &points, Eigen::Vector2d center, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
    Surface2D(std::vector<glm::vec2> &points, glm::vec2 center, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
    Surface2D(std::vector<glm::vec2> &points, glm::vec2 center, std::vector<glm::vec2> texUV, std::vector<std::string> texNames);

    Surface2D();

    void setMesh(std::vector<glm::vec2> &points, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
    void setMesh(std::vector<glm::vec2> &points, std::vector<glm::vec2> texUV, std::vector<std::string> texNames);

    void Draw(std::shared_ptr<Shader> shader, Camera2D &camera);
    void Draw(std::shared_ptr<Shader> shader, Camera2D &camera, glm::vec3 color);

    void Rotate(float angle);
    void Translate(glm::vec2 v);
    void Scale(glm::vec2 v);
    void Flipped(bool flipped);

    void setPosition(Eigen::Vector2d v);
    void setPosition(glm::vec2 v);
    void setPositionRotation(Eigen::Vector2d v, float angle);

    glm::vec2 getScale();
};

Surface2D Ellipse(float a, float b, glm::vec2 center, glm::vec3 color = glm::vec3(1.f), int num = 100);

Surface2D Circle(float r, glm::vec2 center, glm::vec3 color = glm::vec3(1.f), int num = 100);

Surface2D Wedge(float r, glm::vec2 center, float angleSize, float direction = 0, glm::vec3 color = glm::vec3(1.f), int num = 100);

Surface2D Rectangle(float a, float b, glm::vec2 center, glm::vec3 color = glm::vec3(1.f), bool centered = false);
Surface2D Rectangle(float a, float b, glm::vec2 center, std::vector<std::string> texNames, bool centered = false, bool flipped = false);

class Line : public Surface2D
{
private:
    float thickness;
    double lenght;

public:
    Line(Eigen::Vector2d start, Eigen::Vector2d end, float thickness, glm::vec3 color = glm::vec3(0, 0, 0));
    Line(glm::vec2 start, glm::vec2 end, float thickness, glm::vec3 color = glm::vec3(0, 0, 0));
    Line();

    void Move(Eigen::Vector2d start, Eigen::Vector2d end);
    void Move(glm::vec2 start, glm::vec2 end);
    double getLenght();
};

#endif