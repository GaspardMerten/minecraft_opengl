//
// Created by gaspa on 28/12/2022.
//

#ifndef OPENGLPROJECT_CAMERA_H
#define OPENGLPROJECT_CAMERA_H

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "../transform/Transform.h"


class Camera {

public:
    explicit Camera(Transform &transform);

    Transform& transform;

    glm::mat4 getViewMatrix();

    static glm::mat4 getProjectionMatrix();

    static float zoom;

    void toggleFirstPerson();

    float firstPersonRotation = 0;

    bool firstPerson = false;
    const glm::vec3 firstPersonDelta = glm::vec3(0, 2, -0.5);
};


#endif //OPENGLPROJECT_CAMERA_H
