
//
// Created by gaspa on 28/12/2022.
//

#ifndef OPENGLPROJECT_WORLD_H
#define OPENGLPROJECT_WORLD_H


#include <map>
#include "glm/fwd.hpp"
#include "glm/detail/type_mat3x3.hpp"
#include "../objects/game_object/GameObject.h"

class World {
public:

    explicit World(std::map<std::tuple<int, int, int>, int> map);

    std::map<std::tuple<int, int, int>, GameObject*> worldBlockInstances;
public:
    std::map<std::tuple<int, int, int>, int> worldBlocks;

    void instantiateObjects(Shader &baseShader, const std::string& blockMesh);

    void draw();
};


#endif //OPENGLPROJECT_WORLD_H
