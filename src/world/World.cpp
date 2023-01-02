//
// Created by gaspa on 28/12/2022.
//

#include "World.h"
#include "../texture/manager/TextureManager.h"
#include "../objects/mesh/manager/MeshManager.h"

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

GameObject* World::getBlockAt(glm::vec3 &vec) {
    const std::map<std::tuple<int, int, int>, GameObject *>::iterator &findIterator = worldBlockInstances.find(
            std::make_tuple(vec.x, vec.z, vec.y));

    if (findIterator != worldBlockInstances.end()) {
        return findIterator->second;
    }
    return nullptr;
}
