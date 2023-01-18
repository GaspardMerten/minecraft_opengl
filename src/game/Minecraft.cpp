//
// Created by gaspa on 31/12/2022.
//

#include "Minecraft.h"
#include "../utils/world/generate_world.h"
#include "../objects/mesh/manager/MeshManager.h"
#include "../texture/manager/TextureManager.h"
#include "../objects/player/Player.h"

Minecraft::Minecraft(int width, int height, int depth, int nbrTrees, int nbCircles, glm::vec3 playerSpawn, GLFWwindow *window) : world(
        generateFlatWorld(width, height, depth, nbrTrees, nbCircles)) {
    world->create();
    pnjManager = new PNJManager(world);
    physicsManager = new PhysicsManager(world);

    auto block = new Player();
    player = block;
    block->player->transform.position = playerSpawn;
    block->player->transform.markAsDirtyState();
    block->collider = Collider{0.5f, 0.5f, 1.0f};

    toRender.push_back(block);
    physicsManager->linkGameObject(block);


    camera = new Camera(block->player->transform);
    cameraControls = new CameraControls(*camera, window);
    playerControls = new PlayerControls(block, *camera, *world);
    double size = 0.6;

    for (int i = 0; i < 20; i++) {
        if (i > 10) {
            size = 0.3;
        }
        auto* sheep = new GameObject(MeshManager::getMesh(MeshType::SHEEP));
        sheep->setTextureID(TextureManager::getTextureID(TextureType::WHITE_SHEEP));
        sheep->transform.setPosition(10, 1, 10);
        sheep->transform.setScale(size, size, size);
        sheep->collider = Collider{2.0f, 2.0f, 0.5f};
        physicsManager->linkGameObject(sheep);
        pnjManager->addPNJ(sheep, {1, 50, 0.1});
        toRender.push_back(sheep);
    }

    size = 2;
    for (int i = 0; i < 20; i++) {
        if (i > 10) {
            size = 1.5;
        }
        auto* sheep = new GameObject(MeshManager::getMesh(MeshType::VILLAGER));
        sheep->setTextureID(TextureManager::getTextureID(TextureType::BROWN_VILLAGER));
        sheep->transform.setPosition(10, 1, 10);
        sheep->transform.setScale(size, size, size);
        sheep->collider = Collider{2.0f, 2.0f, 0.5f};
        physicsManager->linkGameObject(sheep);
        pnjManager->addPNJ(sheep, {1, 50, 0.1});
        toRender.push_back(sheep);
    }





    light = new Light(
            glm::vec3(50, 100, 50),
            glm::vec3(0.0, 0.0, 0.0),
            0.5,
            0, // no specular in minecraft
            1,
            5.0,
            0.0014,
            0.00001,
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

    world->draw(shader, player->getTransform()->getPosition());
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

void Minecraft::processEvents(GLFWwindow *window, Shader &shader) {
    cameraControls->processEvents(window);
    playerControls->processEvents(window, shader);
}

void Minecraft::configureMatrices(Shader &shader) const {
    shader.setMatrix4("V", camera->getViewMatrix());
    shader.setMatrix4("P", camera->getProjectionMatrix());
}

void Minecraft::updateManagers() {
    pnjManager->update();
    physicsManager->update();
}
