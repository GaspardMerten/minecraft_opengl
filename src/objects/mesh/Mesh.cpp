//
// Created by gaspa on 27/12/2022.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "Mesh.h"

void Mesh::loadMesh(const std::string &path) {
    std::ifstream infile(path);

    std::string line;

    auto positions = std::vector<glm::vec3>();
    auto textures = std::vector<glm::vec2>();
    auto normals = std::vector<glm::vec3>();

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string indice;
        iss >> indice;

        if (indice == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            positions.emplace_back(x, y, z);

        } else if (indice == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            normals.emplace_back(x, y, z);
        } else if (indice == "vt") {
            float u, v;
            iss >> u >> v;
            textures.emplace_back(u, v);
        } else if (indice == "f") {
            std::string f1, f2, f3;

            iss >> f1 >> f2 >> f3;

            std::string p, t, n;

            //for face 1
            extract(f1, p, t, n);
            MeshVertex v1 = getVertex(positions, textures, normals, p, t, n);
            vertices.push_back(v1);

            //for face 12
            extract(f2, p, t, n);
            MeshVertex v2 = getVertex(positions, textures, normals, p, t, n);
            vertices.push_back(v2);

            //for face 3
            extract(f3, p, t, n);
            MeshVertex v3 = getVertex(positions, textures, normals, p, t, n);
            vertices.push_back(v3);
        }
    }
}

MeshVertex Mesh::getVertex(std::vector<glm::vec3> &positions,
                           std::vector<glm::vec2> &textures,
                           std::vector<glm::vec3> &normals, const std::string &p,
                           const std::string &t, const std::string &n) {
    MeshVertex v3{};
    v3.position = positions.at((int) std::stof(p) - 1);
    v3.normal = normals.at((int) std::stof(n) - 1);
    v3.texture = textures.at((int) std::stof(t) - 1);
    return v3;
}

void Mesh::extract(std::string &f, std::string &p, std::string &t, std::string &n) {
    p = f.substr(0, f.find('/'));
    f.erase(0, f.find('/') + 1);

    t = f.substr(0, f.find('/'));
    f.erase(0, f.find('/') + 1);

    n = f.substr(0, f.find('/'));
}

bool Mesh::isLoaded() {
    return !vertices.empty();
}

int Mesh::getVerticesCount() {
    return (int) vertices.size();
}



float *Mesh::toFloatArray() {
    int verticesCount = getVerticesCount();
    auto* data = new float[8 * verticesCount];
    
    for (int i = 0; i < verticesCount; i++) {
        MeshVertex v = vertices.at(i);
        data[i * 8] = v.position.x;
        data[i * 8 + 1] = v.position.y;
        data[i * 8 + 2] = v.position.z;

        data[i * 8 + 3] = v.texture.x;
        data[i * 8 + 4] = v.texture.y;

        data[i * 8 + 5] = v.normal.x;
        data[i * 8 + 6] = v.normal.y;
        data[i * 8 + 7] = v.normal.z;
    }


    return data;
}

int Mesh::getFloatArraySize() {
    return 8 * getVerticesCount() * (int) sizeof(float);
}
