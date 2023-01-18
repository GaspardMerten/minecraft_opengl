//
// Created by gaspa on 27/12/2022.
//

#ifndef OPENGLPROJECT_GAMEOBJECT_H
#define OPENGLPROJECT_GAMEOBJECT_H


#include "../transform/Transform.h"
#include "../mesh/Mesh.h"
#include "../renderer/Renderer.h"
#include "../collider/Collider.h"
#include "../physics/PhysicsData.h"
#include "IGameObject.h"

class GameObject : public IGameObject {
public:
    explicit GameObject(const std::string &pathToMesh);

    explicit GameObject(Mesh *mesh);

    GameObject(Mesh *pMesh, Transform& transform);

    int loadMesh(const std::string& path);


    Mesh* mesh{};
    Renderer renderer;
    Transform transform;
    GLuint textureID{};
    Collider collider{};
    PhysicsData physicsData{};

    void setTextureID(GLuint textureID);

    void draw(Shader& shader) override;

    void makeObject(Shader& shader) override;

    void setCollider(Collider newCollider);

    void setPhysicsData(PhysicsData newPhysicsData);

    void makeObject(Shader shader, Renderer& renderer);

    Transform * getTransform()  override;
};


#endif //OPENGLPROJECT_GAMEOBJECT_H
