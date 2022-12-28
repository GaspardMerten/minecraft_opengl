//
// Created by gaspa on 27/12/2022.
//

#include "Transform.h"
#include "glm/ext/matrix_transform.hpp"

Transform::Transform(glm::mat4 modelMatrix) {
    this->model = modelMatrix;
}

void Transform::setPosition(double x, double y, double z) {
    isDirty = true;
    this->position = glm::vec3(x, y, z);
}

void Transform::setPosition(double x, double y) {
    setPosition(x, y, 1);
}

void Transform::setRotation(double x, double y, double z) {
    markAsDirtyState();
    this->rotation = glm::vec3(x, y, z);
}

void Transform::setRotation(double x, double y) {
    setRotation(x, y, 0);
}

void Transform::setScale(double x, double y, double z) {
    markAsDirtyState();
    this->scaling = glm::vec3(x, y, z);
}

void Transform::markAsDirtyState() { isDirty = true; }

void Transform::setScale(double x, double y) {
    setScale(x, y, 1);
}

void Transform::translate(double x, double y, double z) {
    setPosition(this->position.x + x, this->position.y + y, this->position.z + z);
}

void Transform::translate(double x, double y) {
    setPosition(this->position.x + x, this->position.y + y, this->position.z);
}

glm::mat4 Transform::getModel()  {
    if (isDirty) {
        model = glm::mat4(1.0);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scaling);
        isDirty = false;
    }

    return model;
}

void Transform::setRotationZ(double z) {
    setRotation(rotation.x, rotation.y, z);
}

void Transform::rotateZ(double z) {
    setRotationZ(rotation.z + z);
}

void Transform::setRotationY(double y) {
    setRotation(rotation.x, y, rotation.z);
}

void Transform::rotateY(double y) {
    setRotationY(rotation.y + y);
}


void Transform::setRotationX(double x) {
    setRotation(x, rotation.y, rotation.z);
}

void Transform::rotateX(double x) {
    setRotationX(rotation.x + x);
}

glm::vec3 Transform::getPosition() {
    return position;
}

Transform::Transform() {
    this->model = glm::mat4(1.0);
}
