//
// Created by gaspa on 28/12/2022.
//

#ifndef OPENGLPROJECT_CAMERA_H
#define OPENGLPROJECT_CAMERA_H


#include "../transform/Transform.h"

class Camera {
    float zoom = 1.0f;
public:
    explicit Camera(Transform &transform);

    Transform& transform;

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix();

};


#endif //OPENGLPROJECT_CAMERA_H
