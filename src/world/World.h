
//
// Created by gaspa on 28/12/2022.
//

#ifndef OPENGLPROJECT_WORLD_H
#define OPENGLPROJECT_WORLD_H


#include <map>
#include "glm/fwd.hpp"
#include "glm/detail/type_mat3x3.hpp"
#include "../objects/game_object/GameObject.h"
#include "../objects/mesh/manager/MeshManager.h"
#include "../texture/manager/TextureManager.h"

class World {
public:
    World(std::map<std::tuple<int, int, int>, std::tuple<int, MeshType, TextureType>> map, int length, int width, int depth);

    std::map<std::tuple<int, int, int>, GameObject*> worldBlockInstances;
public:
    int width{}, length{}, depth{};


    std::map<std::tuple<int, int, int>, std::tuple<int, MeshType, TextureType>> worldBlocks;



    void create();

    float normalizeAngle(float angle);

    glm::vec3 rayCastingBlockPos(glm::vec3 playerPos, glm::vec3 playerRot);

    glm::vec3 rayCastingGetLowestBlock(glm::vec3 playerPos, glm::vec3 playerRot);

    glm::vec3 rayCastingGetHighestBlock(glm::vec3 playerPos, glm::vec3 playerRot);

    void draw(Shader& shader);

    void removeBlock(glm::vec3 blockPos);


    void makeObjects(Shader &shader);

    bool collides(GameObject* object);

    GameObject *getBlockAt(glm::vec3 &vec);

    void addBlock(glm::vec3 blockPos, Shader &shader);
};


#endif //OPENGLPROJECT_WORLD_H
