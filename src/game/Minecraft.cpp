//
// Created by gaspa on 31/12/2022.
//

#include "Minecraft.h"
#include "../utils/world/generate_world.h"
#include "../objects/mesh/manager/MeshManager.h"
#include "../texture/manager/TextureManager.h"

Minecraft::Minecraft(int width, int height, int depth, glm::vec3 playerSpawn, GLFWwindow *window) : world(
        generateFlatWorld(width, height, depth)) {
    world->create();
    player = new GameObject(MeshManager::getMesh(MeshType::HUMAN));
    player->setTextureID(TextureManager::getTextureID(TextureType::PLAYER));
    player->transform.setPosition(playerSpawn.x, playerSpawn.y, playerSpawn.z);
    camera = new Camera(player->transform);
    cameraControls = new CameraControls(*camera, window);
    playerControls = new PlayerControls(player->transform, *camera, *world);

    auto* sheep = new GameObject(MeshManager::getMesh(MeshType::SHEEP));

    sheep->setTextureID(TextureManager::getTextureID(TextureType::WHITE_SHEEP));
    sheep->transform.setPosition(10, 1, 10);
    sheep->transform.setScale(0.6, 0.6, 0.6);
    auto* cube = new GameObject(MeshManager::getMesh(MeshType::BLOCK));
    cube->setTextureID(TextureManager::getTextureID(TextureType::GLOW_STONE));
    toRender.push_back(cube);
    toRender.push_back(sheep);

    light = new Light(
            glm::vec3(22, 10, 40),
            glm::vec3(0.0, 0.0, 0.0),
            0.5,
            0.8,
            0.5,
            5.0,
            0.014,
            0,
            1.0
    );

    cube->transform.setPosition(22, 10, 40);
}

void Minecraft::render(Shader &shader) {
    if (shader.withLight) {
        light->linkShader(shader);
    }
    if (shader.withLight) {
        light->use(shader);
    }


    world->draw(shader);
    player->draw(shader);
    for (auto &gameObject : toRender) {
        gameObject->draw(shader);
    }
}

void Minecraft::linkShader(Shader &shader) {
    world->makeObjects(shader);
    player->makeObject(shader);
    for (auto &gameObject : toRender) {
        gameObject->makeObject(shader);
    }
}

void Minecraft::processEvents(GLFWwindow *window) {
    playerControls->processEvents(window);
}

void Minecraft::configureMatrices(Shader &shader) const {
    shader.setMatrix4("V", camera->getViewMatrix());
    shader.setMatrix4("P", camera->getProjectionMatrix());
}
