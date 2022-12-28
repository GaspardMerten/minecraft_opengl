//
// Created by gaspa on 28/12/2022.
//

#include "World.h"

#include <utility>

World::World(std::map<std::tuple<int, int, int>, int> map) {
    worldBlocks = std::move(map);
}

void World::instantiateObjects(Shader &baseShader, const std::string& blockMesh) {
    Mesh mesh = Mesh(blockMesh);
    Texture dirtTexture = Texture("resources/textures/dirt.png");
    for (auto &worldBlock : worldBlocks) {
        worldBlockInstances[worldBlock.first] = new GameObject(mesh, baseShader);
        worldBlockInstances[worldBlock.first]->makeObject();
        worldBlockInstances[worldBlock.first]->setTextureID(dirtTexture.getID());
        worldBlockInstances[worldBlock.first]->transform.setPosition(std::get<0>(worldBlock.first), std::get<2>(worldBlock.first), std::get<1>(worldBlock.first));
    }
}

void World::draw() {
    for (auto &worldBlockInstance : worldBlockInstances) {
        // Print block
        worldBlockInstance.second->draw();
    }
}
