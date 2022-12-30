//
// Created by gaspa on 27/12/2022.
//

#ifndef OPENGLPROJECT_GAMEOBJECT_H
#define OPENGLPROJECT_GAMEOBJECT_H


#include "../transform/Transform.h"
#include "../mesh/Mesh.h"
#include "../renderer/Renderer.h"

class GameObject {
public:
    explicit GameObject(const std::string &pathToMesh);

    explicit GameObject(Mesh &mesh);

    int loadMesh(const std::string& path);


    Mesh mesh;
    Renderer renderer;
    Transform transform;
    GLuint textureID{};

    void setTextureID(GLuint textureID);

    void draw(Shader& shader);

    void makeObject(Shader& shader);

};


#endif //OPENGLPROJECT_GAMEOBJECT_H
