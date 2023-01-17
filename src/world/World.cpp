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
        if (glm::distance(playerPosition, worldBlockInstance.second->transform.getPosition()) < 100) {
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
