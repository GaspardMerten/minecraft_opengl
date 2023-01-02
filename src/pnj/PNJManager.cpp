#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
//
// Created by gaspa on 02/01/2023.
//

#include "PNJManager.h"

void PNJManager::addPNJ(GameObject *pnj, PNJData data) {
    pnjS[pnj] = data;
}

void PNJManager::update() {
    for (auto gameObjectAndData: pnjS) {
        GameObject *gameObject = gameObjectAndData.first;
        PNJData data = gameObjectAndData.second;

        if ((float)rand() / (float)RAND_MAX > 0.995 && gameObject->physicsData.velocity == 0) {
            gameObject->physicsData.acceleration = 0.06;
        }

        if (destinations.count(gameObject)) {
            double distance = glm::distance(gameObject->transform.position, destinations[gameObject]);

            if (distance < 0.5) {
                destinations.erase(gameObject);
            } else {
                // move pnj closer to destination
                glm::vec3 direction = glm::normalize(destinations[gameObject] - gameObject->transform.position);
                glm::vec3 delta = direction * data.speed;
                gameObject->transform.translatePure(delta.x, delta.y, delta.z);
                if (world->collides(gameObject)) {
                    gameObject->transform.translatePure(-delta.x, -delta.y, -delta.z);
                    destinations.erase(gameObject);
                }
            }
            continue;
        }  else if (waiting.count(gameObject)) {
            waiting[gameObject]--;
            if (waiting[gameObject] <= 0) {
                waiting.erase(gameObject);
                destinations[gameObject] = glm::vec3(rand() % world->length, 1, rand() % world->width);
                glm::vec3 direction = glm::normalize(destinations[gameObject] - gameObject->transform.position);
                gameObject->transform.setDirection(direction);
            }
            continue;
        } else {
            waiting[gameObject] = rand() % (data.maxWaitTime - data.minWaitTime) + data.minWaitTime;
        }
    }
}

void PNJManager::removePNJ(GameObject *pnj) {

}

PNJManager::PNJManager(World *world) {
    this->world = world;
}


#pragma clang diagnostic pop