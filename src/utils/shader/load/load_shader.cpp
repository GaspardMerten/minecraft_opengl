//
// Created by gaspa on 27/12/2022.
//

#include <string>
#include <fstream>
#include <iostream>
#include "glad/glad.h"

GLuint loadShaderFromFile(std::string filePath, GLenum shaderType) {
    std::ifstream ifs(filePath);

    // check if file exists
    if (!ifs.is_open()) {
        std::cout << "Error: file " << filePath << " not found" << std::endl;
        return 0;
    }

    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));


    GLuint shader = glCreateShader(shaderType);
    GLint  success;
    const char *shaderSource = content.c_str();
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // retrieve compile status from shader (stored into success)
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
    }

    return shader;
}
