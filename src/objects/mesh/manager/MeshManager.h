//
// Created by gaspa on 28/12/2022.
//

#ifndef OPENGLPROJECT_MESHMANAGER_H
#define OPENGLPROJECT_MESHMANAGER_H

#include <string>
#include <map>
#include "glad/glad.h"
#include "../Mesh.h"


enum MeshType {
    HUMAN,
    BLOCK,
    SHEEP, VILLAGER, CUBEMAP
};


class MeshManager {
    static std::map<MeshType, Mesh *> meshes;
public:
    static void linkMesh(MeshType type, const char *path);

    static Mesh *getMesh(MeshType type);
};


#endif //OPENGLPROJECT_MESHMANAGER_H
