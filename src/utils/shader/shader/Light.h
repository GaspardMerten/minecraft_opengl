//
// Created by Jonathan Stefanov on 28/12/2022.
//

#ifndef OPENGLPROJECT_LIGHT_H
#define OPENGLPROJECT_LIGHT_H


#include "Shader.h"

class Light {
public:
    Shader shader;
    glm::vec3 position;
    glm::vec3 materialColor;
    float ambientStrength;
    float specularStrength;
    float diffuseStrength;
    float shininess;

    float linear;
    float quadratic;
    float constant;

    Light(Shader* shader, glm::vec3 position, glm::vec3 materialColor, float ambientStrength, float specularStrength, float diffuseStrength, float shininess, float linear, float quadratic, float constant);

    void init();
    void use();

};


#endif //OPENGLPROJECT_LIGHT_H
