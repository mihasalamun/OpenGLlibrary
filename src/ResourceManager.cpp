#include "ResourceManager.h"

// Instantiate static variables
std::map<std::string, std::shared_ptr<Shader>> ResourceManager::m_shaders;
std::map<std::string, std::shared_ptr<Texture>> ResourceManager::m_textures;
std::map<std::string, std::shared_ptr<Sound>> ResourceManager::m_sounds;
std::map<std::string, std::shared_ptr<HRTF>> ResourceManager::m_hrtfs;
std::map<std::string, json> ResourceManager::m_json;

ResourceManager::~ResourceManager()
{
    Free();
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const char *shaderFile, std::string name, bool geometryShader)
{
    m_shaders.emplace(name, std::shared_ptr<Shader>(new Shader(shaderFile, geometryShader)));
    return m_shaders[name];
}

std::shared_ptr<Shader> ResourceManager::GetShader(std::string name)
{
    if (m_shaders.contains(name))
        return m_shaders[name];
    else
    {
        std::cerr << "No shader by that name: " << name << std::endl;
        exit(1);
    }
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(TextureData textureData, std::string name)
{
    m_textures.emplace(name, std::shared_ptr<Texture>(new Texture(textureData)));
    return m_textures[name];
}

std::shared_ptr<Texture> ResourceManager::GetTexture(std::string name)
{
    if (m_textures.contains(name))
        return m_textures[name];
    else
    {
        std::cerr << "No texture by that name: " << name << std::endl;
        exit(1);
    }
}

std::shared_ptr<Sound> ResourceManager::LoadSound(std::string soundFile, std::string name)
{
    m_sounds.emplace(name, std::shared_ptr<Sound>(new Sound(soundFile)));
    return m_sounds[name];
}

std::shared_ptr<Sound> ResourceManager::GetSound(std::string name)
{
    if (m_sounds.contains(name))
        return m_sounds[name];
    else
    {
        std::cerr << "No sound by that name: " << name << std::endl;
        exit(1);
    }
}

std::shared_ptr<HRTF> ResourceManager::LoadHRTF(std::string soundFile, std::string name)
{
    m_hrtfs.emplace(name, std::shared_ptr<HRTF>(new HRTF(soundFile)));
    return m_hrtfs[name];
}

std::shared_ptr<HRTF> ResourceManager::GetHRTF(std::string name)
{
    if (m_hrtfs.contains(name))
        return m_hrtfs[name];
    else
    {
        std::cerr << "No HRTF by that name: " << name << std::endl;
        exit(1);
    }
}

json &ResourceManager::LoadJSON(std::string jsonFile, std::string name)
{
    std::ifstream file(jsonFile);
    m_json[name] = json::parse(file);
    file.close();
    return m_json[name];
}

json &ResourceManager::GetJSON(std::string name)
{
    if (m_json.contains(name))
        return m_json[name];
    else
    {
        std::cerr << "No json by that name: " << name << std::endl;
        exit(1);
    }
}

void ResourceManager::Free()
{
    for (auto shader : m_shaders)
        shader.second->Delete();
    for (auto texture : m_textures)
        texture.second->Delete();
}
