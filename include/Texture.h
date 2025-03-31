#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/gl.h>
#include <stb/stb_image.h>

#include "Shader.h"
#include <map>
#include <memory>
#include <array>

enum TextureType
{
    diffuse,
    specular,
    spritesheet,
    cubemap,
    framebuffer
};

struct TextureData
{
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml

    std::string filename;
    TextureType texType;
    GLenum format;
    GLenum pixelType;
    GLuint slot;
    std::array<GLenum, 2> minmaxFilter;
    std::array<GLenum, 3> strWrap;
    std::array<GLfloat, 4> borderClampColor;
    std::pair<int, int> tileDims;

    /**
     * @brief Construct a new Texture Data object
     *
     * @param filename For cubemap and start with 0: right, left, top, bottom, front, back
     * @param texType diffuse, specular, spritesheet, cubemap, framebuffer
     * @param format GL_RGBA, GL_RGB, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL
     * @param pixelType GL_UNSIGNED_BYTE, GL_BYTE, GL_FLOAT
     * @param slot
     * @param minmaxFilter GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
     * @param strWrap GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_REPEAT
     * @param borderClampColor
     * @param tileDims
     */
    TextureData(std::string filename = "MojOpenGL/textures/default.png", TextureType texType = diffuse, GLenum format = GL_RGBA,
                GLenum pixelType = GL_UNSIGNED_BYTE, GLuint slot = -1, std::array<GLenum, 2> minmaxFilter = {GL_LINEAR, GL_LINEAR},
                std::array<GLenum, 3> strWrap = {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE}, std::array<float, 4> borderClampColor = {0, 0, 0, 0},
                std::pair<int, int> tileDims = {60, 60})
        : filename(filename), texType(texType), format(format), pixelType(pixelType), slot(slot), minmaxFilter(minmaxFilter), strWrap(strWrap), borderClampColor(borderClampColor), tileDims(tileDims) {}
};

extern std::map<TextureType, GLenum> TexTypeIDmap;

extern std::map<TextureType, std::string> TexTypeToString;

class Texture
{
private:
    TextureType m_type;
    GLuint m_ID;
    GLuint m_unit;
    GLuint m_TexTypeID;
    GLuint m_RID; // Render buffer ID

public:
    Texture(TextureData texData);
    Texture();

    // Assigns a texture unit to a texture
    void texUnit(std::shared_ptr<Shader> shader, const std::string uniform, GLuint unit);
    // Binds a texture
    void Bind();
    // Unbinds a texture
    void Unbind();
    // Deletes a texture
    void Delete();

    TextureType getType() { return m_type; }
};
#endif