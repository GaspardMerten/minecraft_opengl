//
// Created by gaspa on 28/12/2022.
//

#ifndef OPENGLPROJECT_CAMERACONTROLS_H
#define OPENGLPROJECT_CAMERACONTROLS_H


#include "../../objects/camera/Camera.h"
#include "GLFW/glfw3.h"

class CameraControls {
public:

    explicit CameraControls(Camera &camera, GLFWwindow *window);

    void processEvents(GLFWwindow* glfWindow);

private:
    bool tabWasDown = false;

    Camera &camera;
    GLFWwindow *window;
};


#endif //OPENGLPROJECT_CAMERACONTROLS_H
