//
// Created by Jonathan Stefanov on 28/12/2022.
//

#include "Light.h"


Light::Light(glm::vec3 position, glm::vec3 materialColor, float ambientStrength, float specularStrength,
             float diffuseStrength, float shininess, float linear, float quadratic, float constant) {

    this->transform = new Transform();
    transform->setPosition(position.x, position.y, position.z);
    transform->rotation = glm::vec3(-80, 0, 20);
    transform->markAsDirtyState();

    this->materialColor = materialColor;
    this->ambientStrength = ambientStrength;
    this->specularStrength = specularStrength;
    this->diffuseStrength = diffuseStrength;
    this->shininess = shininess;
    this->linear = linear;
    this->quadratic = quadratic;
    this->constant = constant;
}

void Light::linkShader(Shader& shader) const {
    shader.setFloat("shininess", shininess);
    shader.setVector3f("materialColour", materialColor);
    shader.setFloat("light.ambient_strength", ambientStrength);
    shader.setFloat("light.diffuse_strength", diffuseStrength);
    shader.setFloat("light.specular_strength", specularStrength);
    shader.setFloat("light.constant", constant);
    shader.setFloat("light.linear", linear);
    shader.setFloat("light.quadratic", quadratic);

}

glm::mat4 Light::getSpaceMatrix() const {
    return glm::inverse(transform->getModel());
}
glm::mat4 Light::getOrthoProjectinoMatrix() const {
    return glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);
}

void Light::use(Shader& shader) const {
    shader.setVector3f("light.light_pos", transform->position);
}
