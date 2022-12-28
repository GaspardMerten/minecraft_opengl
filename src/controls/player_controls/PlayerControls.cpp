//
// Created by gaspa on 28/12/2022.
//

#include "PlayerControls.h"

PlayerControls::PlayerControls(Transform &transform, Camera &camera, World &world)
        : transform(transform), camera(camera), world(world) {}

void PlayerControls::processControls(GLFWwindow *window) {
    glm::vec3 oldPosition = this->transform.position;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        transform.translate(-1, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        transform.translate(1, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        transform.translate(0, 0, -1);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        transform.translate(0, 0, 1);
    }
    // space to jump
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        transform.translate(0, 1, 0);
    }
    // shift to go down
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        transform.translate(0, -1, 0);
    }

    if (world.worldBlocks[std::tuple<int, int, int>(transform.position.x, transform.position.z, transform.position.y)] == 1) {
        transform.position = oldPosition;
    }
}


