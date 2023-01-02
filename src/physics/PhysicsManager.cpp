//
// Created by gaspa on 02/01/2023.
//

#include "PhysicsManager.h"

void PhysicsManager::update() {
    for (auto &data : objects) {
        PhysicsData &physicsData = data->physicsData;

        physicsData.velocity += physicsData.acceleration;
        physicsData.velocity -= physicsData.velocity * physicsData.dragCoefficient;

        data->transform.translate(0, physicsData.velocity, 0);

        if (physicsData.velocity != 0 || physicsData.acceleration != 0) {
            physicsData.acceleration -= 0.02;
        }

        if (world->collides(data)) {
            data->transform.translate(0, -physicsData.velocity, 0);
            physicsData.velocity = 0;
            physicsData.acceleration = 0;
        }
    }
}

void PhysicsManager::linkGameObject(GameObject *gameObject) {
    objects.push_back(gameObject);
}

PhysicsManager::PhysicsManager(World *world) : world(world) {}
