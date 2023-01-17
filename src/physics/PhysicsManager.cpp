//
// Created by gaspa on 02/01/2023.
//

#include "PhysicsManager.h"

void PhysicsManager::update() {
    for (auto &data: objects) {
        PhysicsData &physicsData = data->physicsData;

        physicsData.velocity += physicsData.acceleration;
        physicsData.velocity -= physicsData.velocity * physicsData.dragCoefficient;

        data->transform.translate(0, physicsData.velocity, 0);

        GameObject *blockAt = world->getBlockAt(data->transform.position);




        if (physicsData.velocity != 0 && blockAt != nullptr &&
            blockAt->textureID != TextureManager::getTextureID(TextureType::WATER)) {


            data->transform.setPosition(data->transform.position.x, blockAt->transform.position.y + 1,
                                            data->transform.position.z);

            physicsData.velocity = 0;
            physicsData.acceleration = 0;
        } else if (blockAt == nullptr || blockAt->textureID == TextureManager::getTextureID(TextureType::WATER)) {
                physicsData.acceleration = -0.02;
        } else if (physicsData.velocity != 0){
            physicsData.acceleration = -0.02;
        }
    }
}

void PhysicsManager::linkGameObject(GameObject *gameObject) {
    objects.push_back(gameObject);
}

PhysicsManager::PhysicsManager(World *world) : world(world) {}
