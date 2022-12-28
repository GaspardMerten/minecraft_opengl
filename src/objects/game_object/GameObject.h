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
    GameObject(const std::string &pathToMesh, Shader &shader);

    int loadMesh(const std::string& path);

    virtual void draw();

    virtual void makeObject();

    Mesh mesh;
    Renderer renderer;
    Shader& shader;
    Transform transform;
};


#endif //OPENGLPROJECT_GAMEOBJECT_H
