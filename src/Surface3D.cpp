#include "Surface3D.h"

Surface3D::Surface3D(std::vector<std::vector<glm::vec3>> points, std::optional<std::vector<std::vector<glm::vec3>>> normals, glm::vec3 center,
                     std::optional<glm::vec3> color, bool firstPeriodic, bool secondPeriodic) : model(1.0f)
{
    Translate(center);
    setMesh(points, normals, color, firstPeriodic, secondPeriodic);
}

Surface3D::Surface3D(std::vector<Vertex3D> &vertices, std::vector<GLuint> &indicies, glm::vec3 center) : model(1.0f), mesh(vertices, indicies)
{
    Translate(center);
}

Surface3D::Surface3D() {}

void Surface3D::setMesh(std::vector<std::vector<glm::vec3>> points, std::optional<std::vector<std::vector<glm::vec3>>> normals,
                        std::optional<glm::vec3> color, bool firstPeriodic, bool secondPeriodic)
{
    int n1 = points.size();
    int n2 = points[0].size();

    std::vector<Vertex3D> verticies(n1 * n2);

    for (size_t i = 0; i < n1; i++)
    {
        for (size_t j = 0; j < n2; j++)
        {
            verticies[i * n2 + j].position = points[i][j];
            verticies[i * n2 + j].color = color;
            if (normals)
                verticies[i * n2 + j].normal = (*normals)[i][j];
        }
    }

    std::vector<GLuint> indicies((firstPeriodic ? n1 : n1 + 1) * (secondPeriodic ? n2 : n2 + 1) * 2 * 3);

    for (size_t i = 0; i < n1 - 1; i++)
    {
        for (size_t j = 0; j < n2 - 1; j++)
        {
            indicies[6 * (i * n2 + j)] = i * n2 + j;
            indicies[6 * (i * n2 + j) + 2] = i * n2 + j + 1;
            indicies[6 * (i * n2 + j) + 1] = (i + 1) * n2 + j;

            indicies[6 * (i * n2 + j) + 3] = i * n2 + j + 1;
            indicies[6 * (i * n2 + j) + 5] = (i + 1) * n2 + j + 1;
            indicies[6 * (i * n2 + j) + 4] = (i + 1) * n2 + j;
        }
    }

    if (firstPeriodic)
    {
        for (size_t i = 0; i < n1 - 1; i++)
        {
            indicies[6 * ((i + 1) * n2 - 1)] = (i + 1) * n2 - 1;
            indicies[6 * ((i + 1) * n2 - 1) + 2] = i * n2;
            indicies[6 * ((i + 1) * n2 - 1) + 1] = (i + 2) * n2 - 1;

            indicies[6 * ((i + 1) * n2 - 1) + 3] = i * n2;
            indicies[6 * ((i + 1) * n2 - 1) + 5] = (i + 1) * n2;
            indicies[6 * ((i + 1) * n2 - 1) + 4] = (i + 2) * n2 - 1;
        }
    }
    if (secondPeriodic)
    {
        int num12 = (n2 - 1) * n2;
        for (size_t j = 0; j < n2 - 1; j++)
        {
            indicies[6 * (num12 + j)] = num12 + j;
            indicies[6 * (num12 + j) + 2] = num12 + j + 1;
            indicies[6 * (num12 + j) + 1] = j;

            indicies[6 * (num12 + j) + 3] = num12 + j + 1;
            indicies[6 * (num12 + j) + 5] = j + 1;
            indicies[6 * (num12 + j) + 4] = j;
        }
        if (firstPeriodic)
        {
            int num21 = n2 * n2 - 1;
            indicies[6 * num21] = num21;
            indicies[6 * num21 + 1] = num12;
            indicies[6 * num21 + 2] = n2 - 1;

            indicies[6 * num21 + 3] = num12;
            indicies[6 * num21 + 4] = 0;
            indicies[6 * num21 + 5] = n2 - 1;
        }
    }

    mesh.setMesh(verticies, indicies);
}

void Surface3D::Draw(std::shared_ptr<Shader> shader, Camera3D &camera)
{
    shader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    mesh.Draw(shader, camera);
}

void Surface3D::Draw(std::shared_ptr<Shader> shader, Camera3D &camera, glm::vec3 color)
{
    shader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader->getID(), "color"), 1, glm::value_ptr(color));
    mesh.Draw(shader, camera);
}

void Surface3D::Draw(std::shared_ptr<Shader> shader, Camera3D &camera, glm::vec4 color)
{
    shader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform4fv(glGetUniformLocation(shader->getID(), "color"), 1, glm::value_ptr(color));
    mesh.Draw(shader, camera);
}

void Surface3D::DrawWireframe(Camera3D &camera, float scale, glm::vec3 color)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Make it so only the pixels without the value 1 pass the test
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // Disable modifying of the stencil buffer
    glStencilMask(0x00);

    ResourceManager::GetShader("wireframe")->Activate();
    glUniform1f(glGetUniformLocation(ResourceManager::GetShader("wireframe")->getID(), "scale"), scale);
    glUniform3f(glGetUniformLocation(ResourceManager::GetShader("wireframe")->getID(), "color"), color[0], color[1], color[2]);
    Draw(ResourceManager::GetShader("wireframe"), camera);

    // Enable modifying of the stencil buffer
    glStencilMask(0xFF);
    // Clear stencil buffer
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Surface3D::Rotate(float angle, glm::vec3 axis)
{
    model = glm::rotate(model, angle, axis);
}

void Surface3D::Rotate(float angle, glm::vec3 axis, glm::vec3 center)
{
    glm::mat4 rotmat(1);
    rotmat = glm::translate(rotmat, -center);
    rotmat = glm::rotate(rotmat, angle, axis);
    rotmat = glm::translate(rotmat, center);
    model = rotmat * model;
}

void Surface3D::Scale(float scale)
{
    model = glm::scale(model, glm::vec3(scale, scale, scale));
}

void Surface3D::Scale(glm::vec3 scale)
{
    model = glm::scale(model, scale);
}

void Surface3D::Translate(glm::vec3 v)
{
    model = glm::translate(model, v);
}

void Surface3D::setPosition(glm::vec3 pos)
{
    model = glm::mat4(1);
    Translate(pos);
}

void Surface3D::setPosition(Eigen::Vector3d pos)
{
    setPosition(vec3_eigen_to_glm(pos));
}

void Surface3D::setPositionRotation(glm::vec3 pos, glm::vec3 axis)
{
    model = glm::mat4(1);
    Translate(pos);
    model = model * glm::orientation(axis, glm::vec3(0, 1, 0));
}

void Surface3D::setPositionRotation(Eigen::Vector3d pos, Eigen::Vector3d axis)
{
    setPositionRotation(vec3_eigen_to_glm(pos), vec3_eigen_to_glm(axis));
}

Surface3D Ellipsoid(float a, float b, float c, glm::vec3 center, std::optional<glm::vec3> color, int num, bool setNormals)
{
    std::vector<double> phi = linspace<double>(0, 2 * M_PI, num, false);
    std::vector<double> theta = linspace<double>(0, M_PI, num, true);

    std::vector<std::vector<glm::vec3>> points(num, std::vector<glm::vec3>(num));
    std::vector<std::vector<glm::vec3>> normals(num, std::vector<glm::vec3>(num));

    for (size_t i = 0; i < num; i++)
    {
        for (size_t j = 0; j < num; j++)
        {
            float x = sin(theta[i]) * cos(phi[j]);
            float y = sin(theta[i]) * sin(phi[j]);
            float z = cos(theta[i]);
            points[i][j] = glm::vec3(a * x, c * z, b * y);
            if (setNormals)
            {
                normals[i][j] = glm::vec3(b * c * x, a * b * z, a * c * y);
            }
        }
    }
    if (setNormals)
        return Surface3D(points, normals, center, color, true, false);
    else
        return Surface3D(points, std::nullopt, center, color, true, false);
}

Surface3D Sphere(float r, glm::vec3 center, std::optional<glm::vec3> color, int num, bool setNormals)
{
    return Ellipsoid(r, r, r, center, color, num, setNormals);
}

Surface3D Rectangle(glm::vec3 normal, float rotation, glm::vec3 center, float a, float b, std::optional<glm::vec3> color, bool setNormals)
{
    std::vector<std::vector<glm::vec3>> points(2, std::vector<glm::vec3>(2));
    std::vector<std::vector<glm::vec3>> normals(2, std::vector<glm::vec3>(2));
    normal = glm::normalize(normal);
    glm::vec3 vecx = 0.5f * b * glm::normalize(glm::vec3(normal.z, 0, -normal.x));
    glm::vec3 vecy = 0.5f * a * glm::normalize(glm::vec3(0, normal.z, -normal.y));
    vecx = glm::rotate(vecx, rotation, normal);
    vecy = glm::rotate(vecy, rotation, normal);
    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 2; j++)
        {

            glm::vec3 point = (2.0f * i - 1.0f) * vecx + (2.0f * j - 1.0f) * vecy;
            points[i][j] = point;
            points[i][j].y = point.z;
            points[i][j].z = point.y;

            normals[i][j] = normal;
            normals[i][j].y = normal.z;
            normals[i][j].z = normal.y;
        }
    }

    if (setNormals)
        return Surface3D(points, normals, center, color, false, false);
    else
        return Surface3D(points, std::nullopt, center, color, false, false);
}

Surface3D RectCuboid(glm::vec3 center, float a, float b, float c, std::optional<glm::vec3> color)
{
    // TODO: dodaj rotacijo
    // Cube
    std::vector<Vertex3D> cubeVertices =
        {//     COORDINATES     //
         Vertex3D{glm::vec3(-0.5f, -0.5f, 0.5f), color},
         Vertex3D{glm::vec3(-0.5f, -0.5f, -0.5f), color},
         Vertex3D{glm::vec3(0.5f, -0.5f, -0.5f), color},
         Vertex3D{glm::vec3(0.5f, -0.5f, 0.5f), color},
         Vertex3D{glm::vec3(-0.5f, 0.5f, 0.5f), color},
         Vertex3D{glm::vec3(-0.5f, 0.5f, -0.5f), color},
         Vertex3D{glm::vec3(0.5f, 0.5f, -0.5f), color},
         Vertex3D{glm::vec3(0.5f, 0.5f, 0.5f), color}};

    std::vector<GLuint> cubeIndices =
        {
            0, 1, 2,
            0, 2, 3,
            7, 4, 0,
            3, 7, 0,
            6, 7, 3,
            2, 6, 3,
            5, 6, 2,
            1, 5, 2,
            4, 5, 1,
            0, 4, 1,
            6, 5, 4,
            7, 6, 4};

    Surface3D rect(cubeVertices, cubeIndices, center);
    rect.Scale(glm::vec3(a, b, c));
    return rect;
}
