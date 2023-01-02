//
// Created by Jonathan Stefanov on 28/12/2022.
//

#include "Light.h"


Light::Light(glm::vec3 position, glm::vec3 materialColor, float ambientStrength, float specularStrength,
             float diffuseStrength, float shininess, float linear, float quadratic, float constant) {

    this->transform = new Transform();
    transform->setPosition(position.x, position.y, position.z);

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
    shader.setVector3f("light.light_pos", transform->position);

}

glm::mat4 Light::getSpaceMatrix() const {
    const glm::mat4 &viewMatrix = glm::inverse(transform->getModel());

    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 100.0f);

    return viewMatrix * lightProjection;
}

void Light::use(Shader& shader) const {
    shader.setVector3f("light.light_pos", transform->position);
}
