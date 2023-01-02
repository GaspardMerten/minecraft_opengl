
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
    World(std::map<std::tuple<int, int, int>, int> map, int width, int length, int depth);

    std::map<std::tuple<int, int, int>, GameObject*> worldBlockInstances;
public:
    std::map<std::tuple<int, int, int>, int> worldBlocks;

    int width;
    int length;
    int depth;

    void create();

    void draw(Shader& shader);

    void makeObjects(Shader &shader);
};


#endif //OPENGLPROJECT_WORLD_H
