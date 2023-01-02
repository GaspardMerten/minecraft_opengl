//
// Created by gaspa on 28/12/2022.
//

#include "PlayerControls.h"

PlayerControls::PlayerControls(Transform &transform, Camera &camera, World &world)
        : transform(transform), camera(camera), world(world) {}

void PlayerControls::processEvents(GLFWwindow *window) {
    processMouse(window);

    glm::vec3 oldPosition = this->transform.position;

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS) {

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            transform.translate(-speed, 0, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            transform.translate(speed, 0, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            transform.translate(0, 0, -speed);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            transform.translate(0, 0, speed);
        }
        // space to jump
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            transform.translate(0, 1, 0);
        }
        // shift to go down
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            transform.translate(0, -1, 0);
        }
    }

    // rotate if pressing left alt
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            transform.rotateY(2);
        } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            transform.rotateY(-2);
        }
    }

    glm::vec3 &position = transform.position;
    std::vector<glm::vec3> collisionBox = {
            glm::vec3(position.x, position.y, position.z),
            glm::vec3(position.x, position.y + 1, position.z),
            glm::vec3(position.x - 0.25, position.y, position.z - 0.25),
            glm::vec3(position.x + 0.25, position.y, position.z + 0.25),
            glm::vec3(position.x - 0.25, position.y, position.z + 0.25),
            glm::vec3(position.x + 0.25, position.y, position.z - 0.25),
            glm::vec3(position.x - 0.25, position.y + 1, position.z - 0.25),
            glm::vec3(position.x + 0.25, position.y + 1, position.z + 0.25),
            glm::vec3(position.x - 0.25, position.y + 1, position.z + 0.25),
            glm::vec3(position.x + 0.25, position.y + 1, position.z - 0.25),
    };

    bool didCollide = false;

    for (glm::vec3 &vec : collisionBox) {
        if (world.getBlockAt(vec) != nullptr) {
            didCollide = true;
            break;
        }
    }

    if (didCollide) {
        position = oldPosition;
    }
}


void PlayerControls::processMouse(GLFWwindow *window) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    double x, y;

    glfwGetCursorPos(window, &x, &y);

    double delta_x = x - lastX;
    // double delta_y = y - lastY; // strange behaviour (should move camera not player)

    transform.rotateY(-delta_x * mouseSensitivity);

    // if escape is pressed, reset the mouse position
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        glfwSetCursorPos(window, windowWidth/2, windowHeight/2);
    }
    // wrap pointer


    lastX = windowWidth/2;
    lastY = windowHeight/2;
}
