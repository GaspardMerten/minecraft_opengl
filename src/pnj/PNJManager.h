//
// Created by gaspa on 02/01/2023.
//

#ifndef OPENGLPROJECT_PNJMANAGER_H
#define OPENGLPROJECT_PNJMANAGER_H

#include "map"
#include "glm/vec3.hpp"
#include "../objects/game_object/GameObject.h"
#include "PNJData.h"
#include "../world/World.h"

class PNJManager {
    std::map<GameObject*, PNJData> pnjS = std::map<GameObject*, PNJData>();
    std::map<GameObject*, glm::vec3> destinations = std::map<GameObject*, glm::vec3>();
    std::map<GameObject*, int> waiting = std::map<GameObject*, int>();

    World *world{};
public:
    explicit PNJManager(World *world);
    void addPNJ(GameObject* pnj, PNJData data);
    void update();
    void removePNJ(GameObject* pnj);

};


#endif //OPENGLPROJECT_PNJMANAGER_H
