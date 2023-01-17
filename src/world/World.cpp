//
// Created by gaspa on 28/12/2022.
//

#include "World.h"
#include "../texture/manager/TextureManager.h"
#include "../objects/mesh/manager/MeshManager.h"
#include "glm/gtc/quaternion.hpp"

#include <utility>

const int FIELD_OF_VIEW = 100;

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
        worldBlockInstances[worldBlock.first] = new GameObject(MeshManager::getMesh(std::get<1>(worldBlock.second)));
        worldBlockInstances[worldBlock.first]->setTextureID(
                TextureManager::getTextureID(std::get<2>(worldBlock.second)));
        worldBlockInstances[worldBlock.first]->transform.setPosition(std::get<0>(worldBlock.first),
                                                                     std::get<2>(worldBlock.first),
                                                                     std::get<1>(worldBlock.first));
    }
}

void World::makeObjects(Shader &shader) {
    std::map<GLuint, GameObject *> bases = {};
    for (auto &worldBlockInstance: worldBlockInstances) {
        if (bases.find(worldBlockInstance.second->textureID) == bases.end()) {
            bases[worldBlockInstance.second->textureID] = worldBlockInstance.second;
            worldBlockInstance.second->makeObject(shader);
        } else {
            worldBlockInstance.second->makeObject(shader, bases[worldBlockInstance.second->textureID]->renderer);
        }
    }

}

void World::draw(Shader &shader, glm::vec3 playerPosition) {
    for (auto &worldBlockInstance: worldBlockInstances) {
        if (glm::distance(playerPosition, worldBlockInstance.second->transform.getPosition()) < FIELD_OF_VIEW) {
            worldBlockInstance.second->draw(shader);
        }
    }
}

GameObject *World::getBlockAt(glm::vec3 &vec) {
    std::map<std::tuple<int, int, int>, GameObject *>::iterator findIterator;
    if (vec.y < 0) {
        findIterator = worldBlockInstances.find(
                std::make_tuple(vec.x, vec.z, vec.y - 1));

    } else {
        findIterator = worldBlockInstances.find(
                std::make_tuple(vec.x, vec.z, vec.y));

    }

    if (findIterator != worldBlockInstances.end()) {
        return findIterator->second;
    }
    return nullptr;
}

bool World::collides(GameObject *object) {
    glm::vec3 position = object->transform.position;

    glm::vec3 corner = glm::vec3(object->collider.length / 2,
                                 0,
                                 object->collider.width / 2);

    // rotate the corner according to object's rotation
    glm::mat4 rot = glm::rotate(glm::mat4(1), glm::radians(object->transform.rotation.y), glm::vec3(.0f, 1.0f, 0.0f));

    glm::vec3 rotatedCorner = rot * glm::vec4(corner, 1.0f);

    std::vector<glm::vec3> collisionBox = {
            position,
            position - glm::vec3(corner.x, 0, corner.z),
            position + glm::vec3(corner.x, 0, corner.z),
            position + glm::vec3(-corner.x, 0, corner.z),
            position + glm::vec3(corner.x, 0, -corner.z),
            position + glm::vec3(corner.x, 0.05, corner.z),
            position + glm::vec3(-corner.x, 0.05, corner.z),
            position + glm::vec3(corner.x, 0.05, -corner.z),
    };

    bool didCollide = false;

    for (glm::vec3 &vec: collisionBox) {
        GameObject *blockAt = getBlockAt(vec);
        if (blockAt != nullptr && blockAt->textureID != TextureManager::getTextureID(TextureType::WATER)) {
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

    // if raycasting is between 0 and 22,5 degrees
    if (normalizedRotY >= 0 && normalizedRotY < 22.5) {
        rayCastingBlockPos += glm::vec3(0, 3, -1);
    }
    // if raycasting is between 22,5 and 67,5 degrees
    else if (normalizedRotY >= 22.5 && normalizedRotY < 67.5) {
        rayCastingBlockPos += glm::vec3(-1, 3, -1);
    }
    // if raycasting is between 67,5 and 112,5 degrees
    else if (normalizedRotY >= 67.5 && normalizedRotY < 112.5) {
        rayCastingBlockPos += glm::vec3(-1, 3, 0);
    }
    // if raycasting is between 112,5 and 157,5 degrees
    else if (normalizedRotY >= 112.5 && normalizedRotY < 157.5) {
        rayCastingBlockPos += glm::vec3(-1, 3, 1);
    }
    // if raycasting is between 157,5 and 202,5 degrees
    else if (normalizedRotY >= 157.5 && normalizedRotY < 202.5) {
        rayCastingBlockPos += glm::vec3(0, 3, 1);
    }
    // if raycasting is between 202,5 and 247,5 degrees
    else if (normalizedRotY >= 202.5 && normalizedRotY < 247.5) {
        rayCastingBlockPos += glm::vec3(1, 3, 1);
    }
    // if raycasting is between 247,5 and 292,5 degrees
    else if (normalizedRotY >= 247.5 && normalizedRotY < 292.5) {
        rayCastingBlockPos += glm::vec3(1, 3, 0);
    }
    // if raycasting is between 292,5 and 337,5 degrees
    else if (normalizedRotY >= 292.5 && normalizedRotY < 337.5) {
        rayCastingBlockPos += glm::vec3(1, 3, -1);
    }
    // if raycasting is between 337,5 and 360 degrees
    else if (normalizedRotY >= 337.5 && normalizedRotY < 360) {
        rayCastingBlockPos += glm::vec3(0, 3, -1);
    }

    // if rayCastingBlockPos is between 0 and 180
    /*if (normalizedRotY >= 0 && normalizedRotY < 45) {
        rayCastingBlockPos += glm::vec3(0, 3, 1);
        // print the vec3 added glm::vec3(0, 3, 1)
        std::cout << "0 1" << std::endl;
    }
    // between 45 and 135
    else if (normalizedRotY >= 45 && normalizedRotY < 135) {
        rayCastingBlockPos += glm::vec3(-1, 3, 0);
        // print the vec3 added glm::vec3(-1, 3, 0)
        std::cout << "-1 0" << std::endl;

    }
    // between 135 and 225
    else if (normalizedRotY >= 135 && normalizedRotY < 225) {
        rayCastingBlockPos += glm::vec3(0, 3, 1);
        // print the vec3 added glm::vec3(0, 3, 1)
        std::cout << "0 1" << std::endl;
    }
    // between 255 and 315
    else if (normalizedRotY >= 225 && normalizedRotY < 315) {
        rayCastingBlockPos += glm::vec3(1, 3, 0);
        // print the vec3 added glm::vec3(1, 3, 0)
        std::cout << "1 0" << std::endl;
    }
    // between 315 and 360
    else if (normalizedRotY >= 315 && normalizedRotY < 360) {
        rayCastingBlockPos += glm::vec3(0, 3, -1);
        // print the vec3 added glm::vec3(0, 3, -1)
        std::cout << "0 -1" << std::endl;
    }

    // between 0 and 45 degres
    else if (normalizedRotY >= 0 && normalizedRotY < 45) {
        rayCastingBlockPos += glm::vec3(0, 3, 1);
    }*/

    // print rayCastingBlockPos
    // get the lowest block
    return (rayCastingBlockPos);


}

glm::vec3 World::rayCastingGetLowestBlock(glm::vec3 playerPos, glm::vec3 playerRot) {
    glm::vec3 groundPos = rayCastingBlockPos(playerPos, playerRot);
    // Check if there is a block at groundPos
    int i = 0;
    while(worldBlockInstances.count(std::make_tuple(groundPos.x, groundPos.z, groundPos.y)) != 1  && i < 10) {
        // print this positing : block fpound at
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
        worldBlockInstances[std::make_tuple(blockPos.x, blockPos.z, blockPos.y)]->transform.setPosition(blockPos.x, blockPos.y, blockPos.z);

        // make object and draw
        worldBlockInstances[std::make_tuple(blockPos.x, blockPos.z, blockPos.y)]->makeObject(shader);


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


