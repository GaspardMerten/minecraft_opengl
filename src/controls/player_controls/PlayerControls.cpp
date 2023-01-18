//
// Created by gaspa on 28/12/2022.
//

#include "PlayerControls.h"

PlayerControls::PlayerControls(GameObject *player, GameObject *RArm,Camera &camera, World &world)
        : player(player), RArm(RArm), camera(camera), world(world) {}

void PlayerControls::processEvents(GLFWwindow *window, Shader &shader) {
    processMouse(window);

    glm::vec3 oldPosition = player->transform.position;

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            player->transform.translate(-speed, 0, 0);
            RArm->transform.translate(-speed, 0, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            player->transform.translate(speed, 0, 0);
            RArm->transform.translate(speed, 0, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            player->transform.translate(0, 0, -speed);
            RArm->transform.translate(0, 0, -speed);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            player->transform.translate(0, 0, speed);
            RArm->transform.translate(0, 0, speed);
        }
        // space to jump
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (player->physicsData.velocity == 0 && player->physicsData.acceleration == 0) {
            player->physicsData.acceleration = 0.075;
            RArm->physicsData.acceleration = 0.075;
            }
        }
        // shift to go down
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            player->transform.translate(0, -1, 0);
            RArm->transform.translate(0, -1, 0);
        }

        // check if left click
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            // Removing block
            float yRotation = world.normalizeAngle(player->transform.rotation.y);
            // normalize y rotation for it to be between 0 and 360 degrees
            //  one block in front           glm::vec3 blockPos = player->transform.position + glm::vec3(1, -1, 0);
            // get block in front of player
            glm::vec3 blockPos = world.rayCastingGetLowestBlock(player->transform.position, player->transform.rotation);
            // round blockPos
            //blockPos = player->transform.position + glm::vec3(1, -1, 0);
            blockPos.x = round(blockPos.x);
            blockPos.y = round(blockPos.y);
            blockPos.z = round(blockPos.z);
            // remove block
            world.removeBlock(blockPos);
            //print blockPos
            std::cout << "Block removed at " << blockPos.x << " " << blockPos.z << " " << blockPos.y << std::endl;
            // print player position and then rotation
            std::cout << "Player position: " << player->transform.position.x << " " << player->transform.position.z << " " << player->transform.position.y << std::endl;
            std::cout << "Player rotation: " << world.normalizeAngle(player->transform.rotation.y) << std::endl;

        }
        // check if right click
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            // Adding block
            // get y rotation
            float yRotation = world.normalizeAngle(player->transform.rotation.y);
            // normalize y rotation for it to be between 0 and 360 degrees
            //  one block in front           glm::vec3 blockPos = player->transform.position + glm::vec3(1, -1, 0);
            // get block in front of player
            glm::vec3 blockPos = world.rayCastingGetHighestBlock(player->transform.position, player->transform.rotation);

            // round blockPos
            blockPos.x = round(blockPos.x);
            blockPos.y = round(blockPos.y);
            blockPos.z = round(blockPos.z);
            // add block
            world.addBlock(blockPos, shader);
            //print blockPos
            std::cout << "Block added at " << blockPos.x << " " << blockPos.z << " " << blockPos.y << std::endl;
            // print player position and then rotation
            std::cout << "Player position: " << player->transform.position.x << " " << player->transform.position.z << " " << player->transform.position.y << std::endl;
            std::cout << "Player rotation: " << world.normalizeAngle(player->transform.rotation.y) << std::endl;

            // for loop i that goes from 0 to 30
            for(int i = 0; i < 10; i++) {
                RArm->transform.rotateX(i);
                Sleep(10)
            }
            for(int i = 0; i < 10; i++) {
                RArm->transform.rotateX(- i);
            }
        }
    }

    // rotate if pressing left alt
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            player->transform.rotateY(2);
            RArm->transform.rotateY(2);
        } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            player->transform.rotateY(-2);
            RArm->transform.rotateY(-2);
        }
    }


    if (world.collides(player)) {
        player->transform.position = oldPosition;
        RArm->transform.position = oldPosition;
    }
}


void PlayerControls::processMouse(GLFWwindow *window) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    double x, y;

    glfwGetCursorPos(window, &x, &y);

    double delta_x = x - lastX;
    // double delta_y = y - lastY; // strange behaviour (should move camera not player)

    player->transform.rotateY(-delta_x * mouseSensitivity);
    RArm->transform.rotateY(-delta_x * mouseSensitivity);

    // if escape is pressed, reset the mouse position
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
    }
    // wrap pointer


    lastX = windowWidth / 2;
    lastY = windowHeight / 2;
}


