//
// Created by gaspa on 27/12/2022.
//

#ifndef OPENGLPROJECT_GAMEOBJECT_H
#define OPENGLPROJECT_GAMEOBJECT_H


#include "../transform/Transform.h"
#include "../mesh/Mesh.h"
#include "../renderer/Renderer.h"

class GameObject{
public:
    GameObject(const std::string &pathToMesh, Shader &shader);

    int loadMesh(const std::string& path);

    void draw();

    void makeObject();

    Mesh mesh;
    Renderer renderer;
    Transform transform;
    Shader& shader;
};


#endif //OPENGLPROJECT_GAMEOBJECT_H
