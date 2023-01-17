//
// Created by gaspa on 28/12/2022.
//

#include <iostream>
#include "CameraControls.h"
#include "GLFW/glfw3.h"


CameraControls::CameraControls(Camera &camera, GLFWwindow *window) : camera(camera), window(window) {
    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
        Camera::zoom += (float) yoffset / 10;
        Camera::zoom = std::max(std::min(Camera::zoom, 10.0f), 2.0f);
    });
}

void CameraControls::processEvents(GLFWwindow* glfWindow) {
    if (glfwGetKey(glfWindow, GLFW_KEY_TAB) == GLFW_RELEASE && tabWasDown) {
        std::cout << "Tab released" << std::endl;
        camera.toggleFirstPerson();
        tabWasDown = false;
    } else if (glfwGetKey(glfWindow, GLFW_KEY_TAB) == GLFW_PRESS) {
        tabWasDown = true;
    }

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    double x, y;

    glfwGetCursorPos(window, &x, &y);


    float windowCenter = (float) windowHeight / 2;
    std::cout << "windowCenter: " << windowCenter << std::endl;
    camera.firstPersonRotation  += (windowCenter - ((float) y)) * mouseSensitivity;
    std::cout << "camera.firstPersonRotation: " << camera.firstPersonRotation << std::endl;
     if (camera.firstPersonRotation  > 90) {
         camera.firstPersonRotation  = 90;
     } else if (camera.firstPersonRotation  < -90) {
         camera.firstPersonRotation  = -90;
     }
}

