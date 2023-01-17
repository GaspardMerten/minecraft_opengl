//
// Created by gaspa on 28/12/2022.
//

#include "PlayerControls.h"

PlayerControls::PlayerControls(GameObject *player, Camera &camera, World &world)
        : player(player), camera(camera), world(world) {}

void PlayerControls::processEvents(GLFWwindow *window, Shader &shader) {
    processMouse(window);

    glm::vec3 oldPosition = player->transform.position;

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            player->transform.translate(-speed, 0, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            player->transform.translate(speed, 0, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            player->transform.translate(0, 0, -speed);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            player->transform.translate(0, 0, speed);
        }
        // space to jump
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (player->physicsData.velocity == 0 && player->physicsData.acceleration == 0) {
                player->physicsData.velocity = 0.2;
            }
        }
        // shift to go down
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            player->transform.translate(0, -1, 0);
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
            std::cout << "Player position: " << player->transform.position.x << " " << player->transform.position.z
                      << " " << player->transform.position.y << std::endl;
            std::cout << "Player rotation: " << world.normalizeAngle(player->transform.rotation.y) << std::endl;

        }
        // check if right click
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            didClick = true;
        }


        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && didClick) {
            didClick = false;
            std::cout << "Player position: " << player->transform.position.x << " " << player->transform.position.y
                      << " " << player->transform.position.z << std::endl;
            if (camera.firstPerson) {

                std::cout << "Player rotation: " << world.normalizeAngle(player->transform.rotation.y) << std::endl;
                glm::mat4 rotationMatrixY  = glm::rotate(glm::mat4(1.0), glm::radians(180 +player->transform.rotation.y), glm::vec3(0, 1.0f, 0));
                glm::mat4 rotationMatrixX  = glm::rotate(glm::mat4(1.0), glm::radians(-camera.firstPersonRotation), glm::vec3(1.0f, 0, 0));


                glm::vec3 direction4 = glm::vec3( rotationMatrixY*rotationMatrixX * glm::vec4(0, 0, 1, 1));
                direction4 = glm::normalize(direction4);
                const int STEP_PER_UNIT = 10;
                direction4 =  (1.f / (float) STEP_PER_UNIT) * direction4;

                glm::vec3  direction = glm::vec3(direction4.x, direction4.y, direction4.z);

                glm::vec3 currentPoint = player->transform.position + glm::vec3(0, camera.firstPersonDelta.y, 0);

                bool  found = false;

                std::cout << "Direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
                std::cout << "startPoint: " << currentPoint.x << " " << currentPoint.y << " " << currentPoint.z << std::endl;


                for (int i = 0; i < 10*STEP_PER_UNIT; i++) {
                    currentPoint = currentPoint + direction;
                    if (world.getBlockAt(currentPoint) != nullptr) {
                        found = true;
                        std::cout << "Block found at " << currentPoint.x << " " << currentPoint.y << " " << currentPoint.z << std::endl;
                        break;
                    }
                }

                std::cout << "endPoint: " << currentPoint.x << " " << currentPoint.y << " " << currentPoint.z << std::endl;

                if (found) {
                    world.addBlock(currentPoint - direction, shader);
                }
            }
        }
    }

    // rotate if pressing left alt
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            player->transform.rotateY(2);
        } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            player->transform.rotateY(-2);
        }
    }


    if (world.collides(player)) {
        player->transform.position = oldPosition;
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

    // if escape is pressed, reset the mouse position
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
    }
    // wrap pointer


    lastX = windowWidth / 2;
}


