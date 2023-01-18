//
// Created by gaspa on 18/01/2023.
//

#ifndef OPENGLPROJECT_IGAMEOBJECT_H
#define OPENGLPROJECT_IGAMEOBJECT_H

#include "../../utils/shader/shader/Shader.h"
#include "../physics/PhysicsData.h"
#include "../transform/Transform.h"
#include "../collider/Collider.h"

class IGameObject {
public:
    virtual void makeObject(Shader &shader) = 0;
    virtual void draw(Shader &shader) = 0;
    virtual Transform* getTransform()  = 0;

    PhysicsData physicsData{};
    Collider collider{};
};

#endif //OPENGLPROJECT_IGAMEOBJECT_H

