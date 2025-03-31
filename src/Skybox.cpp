#include "Skybox.h"

Skybox::Skybox(TextureData texData) : skyboxBox(RectCuboid(glm::vec3(0, 0, 0), 1, 1, 1)), skyboxShader(ResourceManager::LoadShader("3D/skybox", "skybox")), skyboxTex(texData)
{
}

void Skybox::Draw(Camera3D &camera)
{
    glDepthFunc(GL_LEQUAL);
    skyboxShader->Activate();
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
    // The last row and column affect the translation of the skybox (which we don't want to affect)
    view = glm::mat4(glm::mat3(glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.getOrientation(), camera.getUp())));
    projection = glm::perspective(glm::radians(camera.getFOV()), (float)camera.GetWidth() / camera.GetHeight(), camera.getNearPlane(), camera.getFarPlane());
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // TODO: mogoče je boljše na CPU zmnožit v camMatrix, idk kaj je hitrejše, poglej če se da camMatrix iz katere naredit uporabno

    glActiveTexture(GL_TEXTURE0);
    skyboxTex.Bind();
    skyboxBox.Draw(skyboxShader, camera);
    glDepthFunc(GL_LESS);
}