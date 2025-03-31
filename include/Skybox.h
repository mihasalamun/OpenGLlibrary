#ifndef SKYBOX_H
#define SKYBOX_H

#include "Surface3D.h"
#include "Texture.h"

class Skybox
{
private:
    Surface3D skyboxBox;
    std::shared_ptr<Shader> skyboxShader;
    Texture skyboxTex;

public:
    Skybox(TextureData texData = TextureData("MojOpenGL/textures/skybox.jpg", cubemap, GL_RGB, GL_UNSIGNED_BYTE, -1, {GL_LINEAR, GL_LINEAR}));

    void Draw(Camera3D &camera);
};

#endif