//
// Created by Jonathan Stefanov on 28/12/2022.
//

#include "Light.h"

Light::Light(Shader shader, glm::vec3 position, glm::vec3 materialColor, float ambientStrength, float specularStrength,
             float diffuseStrength, float shininess, float linear, float quadratic, float constant) : shader(shader) {
    this->position = position;
    this->materialColor = materialColor;
    this->ambientStrength = ambientStrength;
    this->specularStrength = specularStrength;
    this->diffuseStrength = diffuseStrength;
    this->shininess = shininess;
    this->linear = linear;
    this->quadratic = quadratic;
    this->constant = constant;
}

void Light::init() {
    shader.setFloat("shininess", shininess);
    shader.setVector3f("materialColour", materialColor);
    shader.setFloat("light.ambient_strength", ambientStrength);
    shader.setFloat("light.diffuse_strength", diffuseStrength);
    shader.setFloat("light.specular_strength", specularStrength);
    shader.setFloat("light.constant", constant);
    shader.setFloat("light.linear", linear);
    shader.setFloat("light.quadratic", quadratic);
}

void Light::use(Camera & camera, glm::mat4 model) const {
    glm::mat4 inverseModel = glm::transpose( glm::inverse(model));

    shader.setMatrix4("itM", inverseModel);
    shader.setVector3f("u_view_pos", camera.transform.position);

}