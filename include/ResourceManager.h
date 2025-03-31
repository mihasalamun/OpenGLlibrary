#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <nlohmann/json.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Sound.h"
#include "HRTF.h"

using json = nlohmann::json;

class ResourceManager
{
private:
    static std::map<std::string, std::shared_ptr<Texture>> m_textures;
    static std::map<std::string, std::shared_ptr<Sound>> m_sounds;
    static std::map<std::string, std::shared_ptr<HRTF>> m_hrtfs;
    static std::map<std::string, json> m_json;

public:
    static std::map<std::string, std::shared_ptr<Shader>> m_shaders;
    ResourceManager() {}
    ~ResourceManager();

    static std::shared_ptr<Shader> LoadShader(const char *shaderFile, std::string name, bool geometryShader = false);
    static std::shared_ptr<Shader> GetShader(std::string name);

    static std::shared_ptr<Texture> LoadTexture(TextureData textureData, std::string name);
    static std::shared_ptr<Texture> GetTexture(std::string name);

    static std::shared_ptr<Sound> LoadSound(std::string soundFile, std::string name);
    static std::shared_ptr<Sound> GetSound(std::string name);

    static std::shared_ptr<HRTF> LoadHRTF(std::string soundFile, std::string name); // naredi enotno stvar za sound
    static std::shared_ptr<HRTF> GetHRTF(std::string name);

    static json &LoadJSON(std::string jsonFile, std::string name);
    static json &GetJSON(std::string name);

    static void Free();
};

#endif