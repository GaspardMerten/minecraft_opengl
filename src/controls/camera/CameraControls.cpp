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

        //print zoom
        std::cout << Camera::zoom << std::endl;
    });
}

