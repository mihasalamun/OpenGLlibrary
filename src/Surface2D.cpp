#include "Surface2D.h"

Surface2D::Surface2D(std::vector<glm::vec2> &points, Eigen::Vector2d center, glm::vec3 color) : model(1.0f)
{
    setPosition(center);
    setMesh(points, color);
}

Surface2D::Surface2D(std::vector<glm::vec2> &points, glm::vec2 center, glm::vec3 color) : model(1.0f)
{
    Translate(center);
    setMesh(points, color);
}

Surface2D::Surface2D(std::vector<glm::vec2> &points, glm::vec2 center, std::vector<glm::vec2> texUV, std::vector<std::string> texNames) : model(1.0f)
{
    Translate(center);
    setMesh(points, texUV, texNames);
}

Surface2D::Surface2D() : model(1.0f) {}

void Surface2D::setMesh(std::vector<glm::vec2> &points, glm::vec3 color)
{
    std::vector<Vertex2D> verticies(points.size());

    for (size_t i = 0; i < points.size(); i++)
    {
        verticies[i] = Vertex2D{points[i], color};
    }

    std::vector<GLuint> indicies(3 * (points.size() - 2));

    for (size_t i = 0; i < points.size() - 2; i++)
    {
        indicies[3 * i] = 0;
        indicies[3 * i + 1] = i + 1;
        indicies[3 * i + 2] = i + 2;
    }
    mesh.setMesh(verticies, indicies);
}

void Surface2D::setMesh(std::vector<glm::vec2> &points, std::vector<glm::vec2> texUV, std::vector<std::string> texNames)
{
    std::vector<Vertex2D> verticies(points.size());

    for (size_t i = 0; i < points.size(); i++)
    {
        verticies[i] = Vertex2D{points[i], std::nullopt, texUV[i]};
    }

    std::vector<GLuint> indicies(3 * (points.size() - 2));

    for (size_t i = 0; i < points.size() - 2; i++)
    {
        indicies[3 * i] = 0;
        indicies[3 * i + 1] = i + 1;
        indicies[3 * i + 2] = i + 2;
    }
    mesh.setMesh(verticies, indicies, texNames);
}

void Surface2D::Draw(std::shared_ptr<Shader> shader, Camera2D &camera)
{
    shader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    mesh.Draw(shader, camera);
}

void Surface2D::Draw(std::shared_ptr<Shader> shader, Camera2D &camera, glm::vec3 color)
{
    shader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader->getID(), "color"), 1, glm::value_ptr(color));
    mesh.Draw(shader, camera);
}

void Surface2D::Rotate(float angle)
{
    model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
}

void Surface2D::Translate(glm::vec2 v)
{
    model = glm::translate(model, {v, 0});
}

void Surface2D::Scale(glm::vec2 v)
{
    model = glm::scale(model, {v, 0});
}

void Surface2D::Flipped(bool flipped)
{
    if (flipped)
    {
        model[0][0] = -1;
        model[2][2] = 0;
    }
    else
    {
        model[0][0] = 1;
        model[2][2] = 1;
    }
}

void Surface2D::setPosition(Eigen::Vector2d v)
{
    setPosition(glm::vec2{v[0], v[1]});
}

void Surface2D::setPosition(glm::vec2 v)
{
    model[3][0] = v[0];
    model[3][1] = v[1];
}

void Surface2D::setPositionRotation(Eigen::Vector2d v, float angle)
{
    model = glm::rotate(glm::translate(glm::mat4(1), {v[0], v[1], 0}), angle, glm::vec3(0, 0, 1));
}

glm::vec2 Surface2D::getScale()
{
    return {model[0][0], model[1][1]};
}

Surface2D Ellipse(float a, float b, glm::vec2 center, glm::vec3 color, int num)
{
    std::vector<double> phi = linspace<double>(0, 2 * M_PI, num, false);

    std::vector<glm::vec2> points(num);

    for (size_t i = 0; i < num; i++)
    {

        float x = a * cos(phi[i]);
        float y = b * sin(phi[i]);
        points[i] = glm::vec2(x, y);
    }

    return Surface2D(points, center, color);
}

Surface2D Circle(float r, glm::vec2 center, glm::vec3 color, int num)
{
    return Ellipse(r, r, center, color, num);
}

Surface2D Wedge(float r, glm::vec2 center, float angleSize, float direction, glm::vec3 color, int num)
{
    std::vector<double> phi = linspace<double>(direction - angleSize / 2, direction + angleSize / 2, num, true);
    std::vector<glm::vec2> points(num + 1);
    points[0] = glm::vec2(0, 0);

    for (size_t i = 1; i < num + 1; i++)
    {
        float x = r * cos(phi[i - 1]);
        float y = r * sin(phi[i - 1]);
        points[i] = glm::vec2(x, y);
    }
    return Surface2D(points, center, color);
}

Surface2D Rectangle(float a, float b, glm::vec2 center, glm::vec3 color, bool centered)
{
    std::vector<glm::vec2> points(4);
    if (centered)
        points = {{-a / 2, -b / 2}, {-a / 2, b / 2}, {a / 2, b / 2}, {a / 2, -b / 2}};
    else
        points = {{0, 0}, {a, 0}, {a, b}, {0, b}};

    Surface2D rect(points, center, color);

    return rect;
}

Surface2D Rectangle(float a, float b, glm::vec2 center, std::vector<std::string> texNames, bool centered, bool flipped)
{
    std::vector<glm::vec2> points(4);
    if (centered)
        points = {{-a / 2, -b / 2}, {-a / 2, b / 2}, {a / 2, b / 2}, {a / 2, -b / 2}};
    else
        points = {{0, 0}, {a, 0}, {a, b}, {0, b}};

    std::vector<glm::vec2> texUV;
    if (flipped)
        texUV = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
    else
        texUV = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

    Surface2D rect(points, center, texUV, texNames);

    return rect;
}

std::vector<glm::vec2> square = {{0.5, 0.5}, {0.5, -0.5}, {-0.5, -0.5}, {-0.5, 0.5}};

Line::Line(Eigen::Vector2d start, Eigen::Vector2d end, float thickness, glm::vec3 color) : Surface2D(square, (start + end) / 2.0f, color), thickness(thickness)
{
    Eigen::Vector2d delta = start - end;
    Rotate(std::atan2(delta(1), delta(0)));
    lenght = delta.norm();
    Scale({lenght, thickness});
}

Line::Line(glm::vec2 start, glm::vec2 end, float thickness, glm::vec3 color) : Line(Eigen::Vector2d(start[0], start[1]), Eigen::Vector2d(end[0], end[1]), thickness, color)
{
}

Line::Line()
{
}

void Line::Move(Eigen::Vector2d start, Eigen::Vector2d end)
{
    model = glm::mat4(1);
    setPosition((start + end) / 2);
    Eigen::Vector2d delta = start - end;
    Rotate(std::atan2(delta(1), delta(0)));
    lenght = delta.norm();
    Scale({lenght, thickness});
}

void Line::Move(glm::vec2 start, glm::vec2 end)
{
    Move(Eigen::Vector2d(start[0], start[1]), Eigen::Vector2d(end[0], end[1]));
}

double Line::getLenght()
{
    return lenght;
}