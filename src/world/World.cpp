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
