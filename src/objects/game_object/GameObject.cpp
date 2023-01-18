//
// Created by gaspa on 27/12/2022.
//

#include "GameObject.h"

int GameObject::loadMesh(const std::string &path) {
    this->mesh = new Mesh(path);

    if (mesh->isLoaded()) {
        // print good
        std::cout << "Mesh loaded successfully" << std::endl;
        return 1;
    }
    std::cout << "Mesh loaded not so  successfully" << std::endl;

    return 0;
}

void GameObject::draw(Shader& shader) {
    this->renderer.draw(shader, this->transform, this->mesh, this->textureID);
}

void GameObject::makeObject(Shader& shader) {
    this->renderer.makeObject(shader, this->mesh, this->transform);
}

GameObject::GameObject(const std::string &pathToMesh)  {
    this->loadMesh(pathToMesh);
}


GameObject::GameObject(Mesh *mesh) : mesh(mesh) {}

void GameObject::setTextureID(GLuint newTextureID) {
    this->textureID = newTextureID;
}

void GameObject::setCollider(Collider newCollider) {
    this->collider = newCollider;
}

void GameObject::setPhysicsData(PhysicsData newPhysicsData) {
    this->physicsData = newPhysicsData;
}

void GameObject::makeObject(Shader shader, Renderer& aRenderer) {
    this->renderer.makeObject(shader, this->mesh, this->transform, aRenderer);

}

GameObject::GameObject(Mesh *pMesh, Transform& transform) {
    this->mesh = pMesh;
    this->transform = Transform(transform);
}

Transform *GameObject::getTransform()  {
    return  &this->transform;
}



