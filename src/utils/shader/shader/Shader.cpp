#include "Shader.h"
#include "../load/load_shader.h"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(std::string &vertexShaderPath, std::string &fragmentShaderPath) {
    GLuint shaderV;
    GLuint shaderF;

    shaderV = loadShaderFromFile(vertexShaderPath, GL_VERTEX_SHADER);
    shaderF = loadShaderFromFile(fragmentShaderPath, GL_FRAGMENT_SHADER);// load shaders path from argv

    ID = compileProgram(shaderV, shaderF);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setInteger(const GLchar *name, GLint value) const {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const GLchar *name, GLfloat value) const {
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z) const {
    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void Shader::setVector3f(const GLchar *name, const glm::vec3 &value) const {
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}

void Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint Shader::compileShader(const std::string &shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const char *code = shaderCode.c_str();
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLchar infoLog[1024];
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::string t = "undetermined";
        if (shaderType == GL_VERTEX_SHADER) {
            t = "vertex shader";
        } else if (shaderType == GL_FRAGMENT_SHADER) {
            t = "fragment shader";
        }
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of the " << t << ": " << shaderType << infoLog << std::endl;
    }
    return shader;
}

GLuint Shader::compileProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint programID = glCreateProgram();

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);


    GLchar infoLog[1024];
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR:  " << infoLog << std::endl;
    }
    return programID;
}
