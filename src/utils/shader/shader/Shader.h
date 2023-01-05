#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/vec3.hpp"
#include "glm/fwd.hpp"

class Shader
{
public:
    GLuint ID;

    Shader(std::string &vertexShaderPath, std::string &fragmentShaderPath, bool withTexture = true, bool withLight = true);

    void use() const;
    void setInteger(const GLchar *name, GLint value) const;
    void setFloat(const GLchar* name, GLfloat value) const;
    void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z) const;
    void setVector3f(const GLchar* name, const glm::vec3& value) const;
    void setMatrix4(const GLchar* name, const glm::mat4& matrix) const;
    void set1i(const GLchar* name, GLint value) const;

    bool withTexture, withLight;

    void setBool(const char *string, bool b);

private:
    static GLuint compileShader(const std::string& shaderCode, GLenum shaderType);

    static GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);
};
#endif