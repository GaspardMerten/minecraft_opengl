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

    GameObject* player;
    World& world;
    Camera& camera;

    float speed = 0.12;
    float mouseSensitivity = 0.05;

    double lastX = 0;
    double lastY = 0;
public:
    PlayerControls(GameObject* transform, Camera &camera, World& world);

    void processEvents(GLFWwindow* window);

    void processMouse(GLFWwindow *window);
};


#endif //OPENGLPROJECT_PLAYERCONTROLS_H
