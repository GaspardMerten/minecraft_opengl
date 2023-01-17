//
// Created by gaspa on 28/12/2022.
//

#include "World.h"
#include "../texture/manager/TextureManager.h"
#include "../objects/mesh/manager/MeshManager.h"
#include "glm/gtc/quaternion.hpp"

#include <utility>
#include <array>

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



    // rotate the corner according to object's rotation
    //glm::mat4 rot = glm::rotate(glm::mat4(1), glm::radians(object->transform.rotation.y), glm::vec3(.0f, 1.0f, 0.0f));

    //glm::vec3 rotatedCorner = rot * glm::vec4(corner, 1.0f);


    // calculate the corner positions
    std::array<glm::vec3, 8> corners = {
            glm::vec3( object->collider.length/2,  object->collider.height/2,  object->collider.width/2),
            glm::vec3(-object->collider.length/2,  object->collider.height/2,  object->collider.width/2),
            glm::vec3( object->collider.length/2,  object->collider.height/2, -object->collider.width/2),
            glm::vec3(-object->collider.length/2,  object->collider.height/2, -object->collider.width/2),
            glm::vec3( object->collider.length/2, -object->collider.height/2,  object->collider.width/2),
            glm::vec3(-object->collider.length/2, -object->collider.height/2,  object->collider.width/2),
            glm::vec3(object-> collider.length/2, -object->collider.height/2, -object->collider.width/2),
            glm::vec3(-object->collider.length/2, -object->collider.height/2, -object->collider.width/2)
    };

    // rotate all corner positions
    glm::mat4 rot = glm::rotate(glm::mat4(1), glm::radians(object->transform.rotation.y), glm::vec3(.0f, 1.0f, 0.0f));
    for(int i=0; i<8; i++)
    {
        glm::vec4 rotated_corner = rot * glm::vec4(corners[i], 1.0f);
        corners[i] = position + glm::vec3(rotated_corner);
    }

    // We now have the correct corners of the hitbox

    // create a bounding box for the object's collision box
    glm::vec3 min = corners[0];
    glm::vec3 max = corners[0];
    for (int i = 1; i < 8; i++) {
        min.x = floor(std::min(min.x, corners[i].x));
        min.y = floor(std::min(min.y, corners[i].y));
        min.z = floor(std::min(min.z, corners[i].z));
        max.x = ceil(std::max(max.x, corners[i].x));
        max.y = ceil(std::max(max.y, corners[i].y));
        max.z = ceil(std::max(max.z, corners[i].z));
    }
    // check if any block around the player
    // make an array of all the blocks around the player
    // print position
    std::cout << "Player position: " << position.x << " " << position.y << " " << position.z << std::endl;
    std::cout << "Min: " << min.x << " " << min.y << " " << min.z << std::endl;
    std::cout << "Max: " << max.x << " " << max.y << " " << max.z << std::endl;
    // check if any block within the bounding box intersects with the object's collision box
    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y = 1; y <= max.y; y++) { // only check blocks above the player
            for (int z = min.z; z <= max.z; z++) {
                glm::vec3 blockPos = glm::vec3(x, y, z);
                // if block is under the player
                if (y < position.y) {
                    if (getBlockAt(blockPos) != nullptr) {
                        return true;
                    }
                }

            }
        }
    }

    return false;
    /*
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

    return didCollide;*/
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


