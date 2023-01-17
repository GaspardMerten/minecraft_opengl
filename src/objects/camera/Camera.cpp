//
// Created by gaspa on 28/12/2022.
//

#include <iostream>
#include "Camera.h"


Camera::Camera(Transform &transform) : transform(transform) {

}
glm::mat4 Camera::getViewMatrix()  {
    glm::mat4 model;
    if (firstPerson) {
        model = glm::translate(transform.getModel(), glm::vec3(0, 2, -0.5));
        // print firstPersonRotation
        std::cout << firstPersonRotation << std::endl;
        model = glm::rotate(model, glm::radians(firstPersonRotation), glm::vec3(1, 0, 0));
    } else {
         model = glm::translate(transform.getModel(), glm::vec3(0, 15/zoom, 50/zoom));

    }
    return glm::inverse(model);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 200.0f);
}

float Camera::zoom = 1;

void Camera::toggleFirstPerson() {
    firstPerson = !firstPerson;
}
