//
// Created by gaspa on 28/12/2022.
//

#include "Camera.h"

Camera::Camera(Transform &transform) : transform(transform) {

}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::mat4();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::mat4();
}
