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

    light = new Light(
            glm::vec3(34, 20, 66),
            glm::vec3(0.0, 0.0, 0.0),
            0.9,
            0.8,
            10.5,
            32.0,
            0.14,
            0.01,
            1.0
    );
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
}

void Minecraft::linkShader(Shader &shader) {
    world->makeObjects(shader);
    player->makeObject(shader);
}

void Minecraft::processEvents(GLFWwindow *window) {
    playerControls->processEvents(window);
}

void Minecraft::configureMatrices(Shader &shader) const {
    shader.setMatrix4("V", camera->getViewMatrix());
    shader.setMatrix4("P", camera->getProjectionMatrix());
}
