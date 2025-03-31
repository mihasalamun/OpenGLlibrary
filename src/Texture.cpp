#include "Texture.h"

std::map<TextureType, GLenum> TexTypeIDmap =
    {
        {diffuse, GL_TEXTURE_2D},
        {specular, GL_TEXTURE_2D},
        {spritesheet, GL_TEXTURE_2D_ARRAY},
        {cubemap, GL_TEXTURE_CUBE_MAP},
        {framebuffer, GL_TEXTURE_2D}};

std::map<TextureType, std::string> TexTypeToString =
    {{diffuse, "diffuse"},
     {specular, "specular"},
     {spritesheet, "spritesheet"},
     {cubemap, "cubemap"},
     {cubemap, "framebuffer"}};

Texture::Texture(TextureData texData) : m_type(texData.texType), m_unit(texData.slot), m_TexTypeID(TexTypeIDmap[m_type])
{
    // Generates an OpenGL texture object
    glGenTextures(1, &m_ID);
    // Assigns the texture to a Texture Unit
    if (m_unit != GLuint(-1))
        glActiveTexture(GL_TEXTURE0 + m_unit);

    glBindTexture(m_TexTypeID, m_ID);

    // Configures the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(m_TexTypeID, GL_TEXTURE_MIN_FILTER, texData.minmaxFilter[0]);
    glTexParameteri(m_TexTypeID, GL_TEXTURE_MAG_FILTER, texData.minmaxFilter[1]);

    // Configures texture wrap
    glTexParameteri(m_TexTypeID, GL_TEXTURE_WRAP_S, texData.strWrap[0]);
    glTexParameteri(m_TexTypeID, GL_TEXTURE_WRAP_T, texData.strWrap[1]);
    if (m_type == cubemap)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, texData.strWrap[2]);

    // Extra lines in case you choose to use GL_CLAMP_TO_BORDER
    // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    if ((texData.strWrap[0] == GL_CLAMP_TO_BORDER) || (texData.strWrap[1] == GL_CLAMP_TO_BORDER) || (texData.strWrap[2] == GL_CLAMP_TO_BORDER))
        glTexParameterfv(m_TexTypeID, GL_TEXTURE_BORDER_COLOR, texData.borderClampColor.data());

    // Stores the width, height, and the number of color channels of the image
    int widthImg, heightImg, numColCh;
    // Flips the image so it appears right side up
    // Reads the image from a file and stores it in bytes
    unsigned char *bytes;
    try
    {
        if (texData.texType != TextureType::cubemap || texData.texType != TextureType::framebuffer)
        {
            stbi_set_flip_vertically_on_load(true);
            bytes = stbi_load(texData.filename.c_str(), &widthImg, &heightImg, &numColCh, 0);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error opening image file: " << texData.filename << std::endl;
        std::cerr << e.what() << '\n';
        exit(1);
    }

    if (texData.texType == diffuse || texData.texType == specular)
    {
        // Assigns the image to the OpenGL Texture object
        glTexImage2D(m_TexTypeID, 0, GL_RGBA, widthImg, heightImg, 0, texData.format, texData.pixelType, bytes);
        // Generates MipMaps
        glGenerateMipmap(m_TexTypeID);

        stbi_image_free(bytes);
    }
    else if (texData.texType == spritesheet)
    {
        int tilesCountWidth = widthImg / texData.tileDims.first;
        int tilesCountHeight = heightImg / texData.tileDims.second;

        // int tileWidth = widthImg / texData.tilesCount.first;
        // int tileHeight = heightImg / texData.tilesCount.second;
        int tiles = tilesCountWidth * tilesCountHeight;

        glTexStorage3D(m_TexTypeID, 1, GL_RGBA8, texData.tileDims.first, texData.tileDims.second, tiles); // 1 je mips count, mogoče pogruntaj kak je s tem

        glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, heightImg);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, widthImg);

        for (size_t i = 0; i < tilesCountWidth; i++)
        {
            for (size_t j = 0; j < tilesCountHeight; j++)
            {
                glTexSubImage3D(m_TexTypeID, 0, 0, 0, (tilesCountHeight - 1 - j) * tilesCountWidth + i, texData.tileDims.first, texData.tileDims.second, 1, texData.format, texData.pixelType, bytes + (j * texData.tileDims.second * widthImg + i * texData.tileDims.first) * 4);
            }
        }
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);

        glGenerateMipmap(m_TexTypeID);

        stbi_image_free(bytes);
    }
    else if (texData.texType == TextureType::cubemap)
    {
        int dot = texData.filename.find(".");
        std::string base = texData.filename.substr(0, dot);
        std::string extension = texData.filename.substr(dot);
        // Cycles through all the textures and attaches them to the cubemap object
        for (unsigned int i = 0; i < 6; i++)
        {
            int width, height, nrChannels;
            unsigned char *data = stbi_load((base + std::to_string(i) + extension).c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                stbi_set_flip_vertically_on_load(false);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Failed to load texture: " << (base + std::to_string(i) + extension) << std::endl;
                stbi_image_free(data);
            }
        }
    }
    else if (texData.texType == TextureType::framebuffer)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1916, 1032, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ID, 0);
    }
    else
    {
        std::cerr << "Wrong texture type!" << std::endl;
        exit(1);
    }

    // Unbinds the OpenGL Texture object so that it can't accidentally be modified
    glBindTexture(m_TexTypeID, 0);
}

Texture::Texture()
{
}

void Texture::texUnit(std::shared_ptr<Shader> shader, const std::string uniform, GLuint unit)
{
    // Gets the location of the uniform
    GLuint texUni = glGetUniformLocation(shader->getID(), uniform.c_str());
    // Shader needs to be activated before changing the value of a uniform
    shader->Activate();
    // Sets the value of the uniform
    glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    if (m_unit != GLuint(-1))
        glActiveTexture(GL_TEXTURE0 + m_unit);
    glBindTexture(m_TexTypeID, m_ID);
}

void Texture::Unbind()
{
    glBindTexture(m_TexTypeID, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &m_ID);
}