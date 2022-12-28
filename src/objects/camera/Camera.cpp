//
// Created by gaspa on 28/12/2022.
//

#include <iostream>
#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

Camera::Camera(Transform &transform) : transform(transform) {

}
glm::mat4 Camera::getViewMatrix()  {
    // get vector in orientation of camera
    glm::vec3 cameraDirection = glm::normalize(transform.rotation);

    auto model = glm::translate(transform.getModel(), glm::vec3(0, 15, 50/zoom));
    return glm::inverse(model);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
}
