//
// Created by Jonathan Stefanov on 28/12/2022.
//

#ifndef OPENGLPROJECT_LIGHT_H
#define OPENGLPROJECT_LIGHT_H


#include "Shader.h"
#include "../../../objects/camera/Camera.h"

class Light {
public:
    Transform* transform;
    glm::vec3 materialColor{};
    float ambientStrength;
    float specularStrength;
    float diffuseStrength;
    float shininess;

    float linear;
    float quadratic;
    float constant;

    Light(glm::vec3 position, glm::vec3 materialColor, float ambientStrength, float specularStrength, float diffuseStrength, float shininess, float linear, float quadratic, float constant);

    void linkShader(Shader &shader) const;

    void use(Shader &shader) const;

    glm::mat4 getSpaceMatrix() const;

    static glm::mat4 getOrthoProjectionMatrix() ;
};


#endif //OPENGLPROJECT_LIGHT_H
