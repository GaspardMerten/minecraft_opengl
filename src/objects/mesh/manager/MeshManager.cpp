//
// Created by gaspa on 28/12/2022.
//

#include <iostream>
#include "MeshManager.h"

void MeshManager::linkMesh(MeshType type, const char* path) {
    meshes[type] = new Mesh(path);
}

Mesh* MeshManager::getMesh(MeshType type) {
    if (meshes.find(type) != meshes.end()) {
        return meshes[type];
    } else {
        std::cout << "Error: texture not found - " << type << std::endl;
        return nullptr;
    }
}

std::map<MeshType, Mesh*> MeshManager::meshes = std::map<MeshType, Mesh*>();