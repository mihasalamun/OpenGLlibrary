#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const std::string filename);

class Shader
{
private:
    GLuint m_ID;

public:
    Shader(const std::string shaderName, bool geometryShader = false);
    ~Shader();

    // Activates the Shader Program
    void Activate();
    // Deletes the Shader Program
    void Delete();

    GLuint getID() { return m_ID; }

private:
    // Checks if the different Shaders have compiled properly
    void compileErrors(unsigned int shader, const char *type);
};

#endif