//
// Created by gaspa on 28/12/2022.
//

#ifndef OPENGLPROJECT_PLAYERCONTROLS_H
#define OPENGLPROJECT_PLAYERCONTROLS_H


#include "../../objects/transform/Transform.h"
#include "../../objects/camera/Camera.h"
#include "../../world/World.h"
#include "GLFW/glfw3.h"

class PlayerControls {

    Transform& transform;
    World& world;
    Camera &camera;

    float speed = 1;
    float mouseSensitivity = 0.1;

public:
    PlayerControls(Transform &transform, Camera &camera, World& world);

    void processControls(GLFWwindow* window);
};


#endif //OPENGLPROJECT_PLAYERCONTROLS_H
