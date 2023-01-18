//
// Created by gaspa on 02/01/2023.
//

#include "PhysicsManager.h"

void PhysicsManager::update() {
    for (auto &data: objects) {
        PhysicsData &physicsData = data->physicsData;

        physicsData.velocity += physicsData.acceleration;
        physicsData.velocity -= physicsData.velocity * physicsData.dragCoefficient;

        data->getTransform()->translate(0, physicsData.velocity, 0);

        GameObject *blockAt = world->getBlockAt(data->getTransform()->position);




        if (physicsData.velocity != 0 && blockAt != nullptr &&
            blockAt->textureID != TextureManager::getTextureID(TextureType::WATER)) {


            data->getTransform()->setPosition(data->getTransform()->position.x, blockAt->transform.position.y + 1,
                                            data->getTransform()->position.z);

            physicsData.velocity = 0;
            physicsData.acceleration = 0;
        } else if (blockAt == nullptr || blockAt->textureID == TextureManager::getTextureID(TextureType::WATER)) {
                physicsData.acceleration = -0.02;
        } else if (physicsData.velocity != 0){
            physicsData.acceleration = -0.02;
        }
    }
}

void PhysicsManager::linkGameObject(IGameObject *gameObject) {
    objects.push_back(gameObject);
}

PhysicsManager::PhysicsManager(World *world) : world(world) {}
