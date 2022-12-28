//
// Created by gaspa on 27/12/2022.
//

#include "GameObject.h"

int GameObject::loadMesh(const std::string &path) {
    this->mesh = Mesh(path);

    if (mesh.isLoaded()) {
        // print good
        std::cout << "Mesh loaded successfully" << std::endl;
        return 1;
    }
    std::cout << "Mesh loaded not so  successfully" << std::endl;

    return 0;
}

void GameObject::draw() {
    this->renderer.draw(shader, this->transform, this->mesh, this->textureID);
}

void GameObject::makeObject() {
    this->renderer.makeObject(shader, this->mesh, this->transform);
}

GameObject::GameObject(const std::string &pathToMesh, Shader &shader) : shader(shader) {
    this->loadMesh(pathToMesh);
}


GameObject::GameObject(Mesh &mesh, Shader &shader) : shader(shader), mesh(mesh) {}

void GameObject::setTextureID(GLuint newTextureID) {
    this->textureID = newTextureID;
}



