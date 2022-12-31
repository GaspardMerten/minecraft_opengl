//
// Created by gaspa on 27/12/2022.
//

#ifndef OPENGLPROJECT_RENDERER_H
#define OPENGLPROJECT_RENDERER_H


#include "glad/glad.h"
#include "../../utils/shader/shader/Shader.h"
#include "../transform/Transform.h"
#include "../mesh/Mesh.h"

class Renderer {
    GLuint VBO{}, VAO{};
public:
    void makeObject(Shader& shader, Mesh * mesh, Transform &transform);

    void draw(Shader& Shader, Transform &transform, Mesh *mesh, GLuint textureID) const;
};


#endif //OPENGLPROJECT_RENDERER_H
