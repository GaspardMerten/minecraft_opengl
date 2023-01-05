//
// Created by gaspa on 05/01/2023.
//

#ifndef OPENGLPROJECT_CUBEMAP_H
#define OPENGLPROJECT_CUBEMAP_H


#include <vector>
#include "glad/glad.h"
#include "../utils/shader/shader/Shader.h"

class CubeMap {
public:
    CubeMap(const std::string &pathSuffix, std::map<std::string, GLenum> &faces);

    static std::vector<float> FacesCoords;

    GLuint textureID{};

    GLuint VBO{}, VAO{};

    void makeObject(Shader &shader);

    void draw(Shader &shader) const;
};


#endif //OPENGLPROJECT_CUBEMAP_H
