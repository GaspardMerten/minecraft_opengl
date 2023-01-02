//
// Created by gaspa on 31/12/2022.
//

#include "Minecraft.h"
#include "../utils/world/generate_world.h"
#include "../objects/mesh/manager/MeshManager.h"
#include "../texture/manager/TextureManager.h"

Minecraft::Minecraft(int width, int height, int depth, int nbrTrees, glm::vec3 playerSpawn, GLFWwindow *window) : world(
        generateFlatWorld(width, height, depth, nbrTrees)) {
    world->create();
    pnjManager = new PNJManager(world);
    physicsManager = new PhysicsManager(world);
    player = new GameObject(MeshManager::getMesh(MeshType::HUMAN));
    player->setTextureID(TextureManager::getTextureID(TextureType::PLAYER));
    player->transform.setPosition(playerSpawn.x, playerSpawn.y, playerSpawn.z);
    player->collider = Collider{0.1f, 0.1f, 0.5f};
    physicsManager->linkGameObject(player);
    camera = new Camera(player->transform);
    cameraControls = new CameraControls(*camera, window);
    playerControls = new PlayerControls(player, *camera, *world);

    for (int i = 0; i < 10; i++) {
        auto* sheep = new GameObject(MeshManager::getMesh(MeshType::SHEEP));
        sheep->setTextureID(TextureManager::getTextureID(TextureType::WHITE_SHEEP));
        sheep->transform.setPosition(10, 1, 10);
        sheep->transform.setScale(0.6, 0.6, 0.6);
        sheep->collider = Collider{2.0f, 2.0f, 0.5f};
        pnjManager->addPNJ(sheep, {1, 50, 0.1});
        toRender.push_back(sheep);

    }


    auto* cube = new GameObject(MeshManager::getMesh(MeshType::BLOCK));
    cube->setTextureID(TextureManager::getTextureID(TextureType::GLOW_STONE));
    toRender.push_back(cube);

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

void Minecraft::updateManagers() {
    pnjManager->update();
    physicsManager->update();
}
