//
// Created by gaspa on 02/01/2023.
//

#ifndef OPENGLPROJECT_PHYSICSMANAGER_H
#define OPENGLPROJECT_PHYSICSMANAGER_H


#include <vector>
#include "../objects/physics/PhysicsData.h"
#include "../objects/game_object/GameObject.h"
#include "../world/World.h"

class PhysicsManager {
    World *world;
    std::vector<IGameObject *> objects = std::vector<IGameObject *>();
public:
    explicit PhysicsManager(World *world);

    void linkGameObject(IGameObject *gameObject);

    void update();
};


#endif //OPENGLPROJECT_PHYSICSMANAGER_H
