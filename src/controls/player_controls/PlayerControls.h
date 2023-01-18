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

    IGameObject* player;
    World& world;
    Camera& camera;

    float speed = 0.2;
    float mouseSensitivity = 0.05;

    double lastX = 0;
    double lastY = 0;
public:
    PlayerControls(IGameObject* transform, Camera &camera, World& world);

    void processEvents(GLFWwindow* window, Shader &shader);

    void processMouse(GLFWwindow *window);

    bool didClick = false;
};


#endif //OPENGLPROJECT_PLAYERCONTROLS_H
