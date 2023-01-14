//
// Created by gaspa on 28/12/2022.
//

#include "World.h"
#include "../texture/manager/TextureManager.h"
#include "../objects/mesh/manager/MeshManager.h"
#include "glm/gtc/quaternion.hpp"

#include <utility>

World::World(std::map<std::tuple<int, int, int>, std::tuple<int, MeshType, TextureType>> map, int length, int width,
             int depth) {
    worldBlocks = std::move(map);
    this->depth = depth;
    this->width = width;
    this->length = length;
}

void World::create() {

    for (auto &worldBlock: worldBlocks) {
        // print blockPos
        std::cout << "Block created at " << std::get<0>(worldBlock.first) << " " << std::get<1>(worldBlock.first) << " " << std::get<2>(worldBlock.first) << std::endl;
        worldBlockInstances[worldBlock.first] = new GameObject(MeshManager::getMesh(std::get<1>(worldBlock.second)));
        worldBlockInstances[worldBlock.first]->setTextureID(
                TextureManager::getTextureID(std::get<2>(worldBlock.second)));
        worldBlockInstances[worldBlock.first]->transform.setPosition(std::get<0>(worldBlock.first),
                                                                     std::get<2>(worldBlock.first),
                                                                     std::get<1>(worldBlock.first));
    }
}

void World::makeObjects(Shader &shader) {
    for (auto &worldBlock: worldBlockInstances) {
        worldBlock.second->makeObject(shader);
    }
}

void World::draw(Shader &shader) {
    for (auto &worldBlockInstance: worldBlockInstances) {
        // Print block
        worldBlockInstance.second->draw(shader);
    }
}

GameObject *World::getBlockAt(glm::vec3 &vec) {
    const std::map<std::tuple<int, int, int>, GameObject *>::iterator &findIterator = worldBlockInstances.find(
            std::make_tuple(vec.x, vec.z, vec.y));

    if (findIterator != worldBlockInstances.end()) {
        return findIterator->second;
    }
    return nullptr;
}

bool World::collides(GameObject *object) {
    glm::vec3 position = object->transform.position;
    position.x += 0.25f;
    position.z += 0.25f;

    glm::vec3 corner = glm::vec3(object->collider.length / 2,
                                 0,
                                 object->collider.width / 2);

    // rotate the corner according to object's rotation
    glm::mat4 rot = glm::rotate(glm::mat4(1), glm::radians(object->transform.rotation.y), glm::vec3(.0f, 1.0f, 0.0f));

    glm::vec3 rotatedCorner = rot * glm::vec4(corner, 1.0f);

    std::vector<glm::vec3> collisionBox = {
            position,
            position-glm::vec3(corner.x, 0, corner.z),
            position+glm::vec3(corner.x, 0, corner.z),
    };

    bool didCollide = false;

    for (glm::vec3 &vec: collisionBox) {
        if (getBlockAt(vec) != nullptr) {
            didCollide = true;
            break;
        }
    }

    return didCollide;
}

void World::removeBlock(glm::vec3 blockPos) {
    worldBlockInstances.erase(std::make_tuple(blockPos.x, blockPos.z, blockPos.y));
}

float World::normalizeAngle(float angle) {
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    return angle;
}

glm::vec3 World::rayCastingBlockPos(glm::vec3 playerPos, glm::vec3 playerRot) {
    float normalizedRotY = normalizeAngle(playerRot.y);
    glm::vec3 rayCastingBlockPos = playerPos;

    // if rayCastingBlockPos is between 0 and 180
    if (normalizedRotY >= 0 && normalizedRotY < 45) {
        rayCastingBlockPos += glm::vec3(0, 3, 1);
    }
    // between 45 and 135
    else if (normalizedRotY >= 45 && normalizedRotY < 135) {
        rayCastingBlockPos += glm::vec3(-2, 3, 0);
    }
    // between 135 and 225
    else if (normalizedRotY >= 135 && normalizedRotY < 225) {
        rayCastingBlockPos += glm::vec3(0, 3, 2);
    }
    // between 255 and 315
    else if (normalizedRotY >= 225 && normalizedRotY < 315) {
        rayCastingBlockPos += glm::vec3(2, 3, 0);
    }
    // between 315 and 360
    else if (normalizedRotY >= 315 && normalizedRotY < 360) {
        rayCastingBlockPos += glm::vec3(0, 3, -2);
    }

    // print rayCastingBlockPos
    std::cout << "RayCastingBlockPos: " << rayCastingBlockPos.x << " " << rayCastingBlockPos.z << " " << rayCastingBlockPos.y << std::endl;
    // get the lowest block
    return (rayCastingBlockPos);


}

glm::vec3 World::rayCastingGetLowestBlock(glm::vec3 playerPos, glm::vec3 playerRot) {
    glm::vec3 groundPos = rayCastingBlockPos(playerPos, playerRot);
    // Check if there is a block at groundPos
    int i = 0;
    while(worldBlockInstances.count(std::make_tuple(groundPos.x, groundPos.z, groundPos.y)) != 1  && i < 10) {
        // print this positing : block fpound at
        std::cout << "Block not found at " << groundPos.x << " " << groundPos.z << " " << groundPos.y << std::endl;
        // while there is a block, go down
        groundPos.y -= 1;
        i++;
    }
    return groundPos;
}

void World::addBlock(glm::vec3 blockPos, Shader &shader) {
    // insert into worldBlocks
    if(!worldBlockInstances.count(std::make_tuple(blockPos.x, blockPos.z, blockPos.y))){
        // No block at this position, can add the block at blockPos
        worldBlockInstances[std::make_tuple(blockPos.x, blockPos.z, blockPos.y)] = new GameObject(MeshManager::getMesh(MeshType::BLOCK));
        worldBlockInstances[std::make_tuple(blockPos.x, blockPos.z, blockPos.y)]->setTextureID(TextureManager::getTextureID(TextureType::DIRT));
        worldBlockInstances[std::make_tuple(blockPos.x, blockPos.z, blockPos.y)]->transform.setPosition(blockPos.x, blockPos.z, blockPos.y);

        // make object and draw
        worldBlockInstances[std::make_tuple(blockPos.x, blockPos.z, blockPos.y)]->makeObject(shader);
        worldBlockInstances[std::make_tuple(blockPos.x, blockPos.z, blockPos.y)]->draw(shader);


    }

}

glm::vec3 World::rayCastingGetHighestBlock(glm::vec3 playerPos, glm::vec3 playerRot) {
    glm::vec3 groundPos = rayCastingBlockPos(playerPos, playerRot) + glm::vec3(0, -3, 0);
    // Check if there is a block at groundPos
    int i = 0;
    while(worldBlockInstances.count(std::make_tuple(groundPos.x, groundPos.z, groundPos.y)) == 1  && i < 10) {
        // print this positing : block fpound at
        // while there is a block, go down
        groundPos.y += 1;
        i++;
    }
    return groundPos;
}


