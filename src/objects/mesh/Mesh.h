//
// Created by gaspa on 27/12/2022.
//

#ifndef OPENGLPROJECT_MESH_H
#define OPENGLPROJECT_MESH_H


#include <vector>
#include <string>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct MeshVertex {
    glm::vec3 position; // position represents the vertex position
    glm::vec2 texture; // texture represents the texture coordinates
    glm::vec3 normal; // normal represents the normal vector
};


class Mesh {

    void loadMesh(const std::string& path);

    static void extract(std::string &f1, std::string &p, std::string &t, std::string &n) ;

    static MeshVertex getVertex(std::vector<glm::vec3> &positions,
    std::vector<glm::vec2> &textures,
            std::vector<glm::vec3> &normals, const std::string &p,
    const std::string &t, const std::string &n) ;

public:
    Mesh() = default;

    explicit Mesh(const std::string& path) {
        loadMesh(path);
    }

    bool isLoaded();

    int getVerticesCount();

    float* toFloatArray();

    int getFloatArraySize();

    std::vector<MeshVertex> vertices;
};


#endif //OPENGLPROJECT_MESH_H
