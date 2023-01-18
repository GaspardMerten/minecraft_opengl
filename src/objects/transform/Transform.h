//
// Created by gaspa on 27/12/2022.
//

#ifndef OPENGLPROJECT_TRANSFORM_H
#define OPENGLPROJECT_TRANSFORM_H


#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"

class Transform {
private:
    glm::vec3 scaling = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 model = glm::mat4(1.0);

    bool isDirty = false;
public:
    explicit Transform(glm::mat4 modelMatrix);

    void translate(double x, double y, double z);

    void setScale(double x, double y, double z);

    void setRotation(double x, double y, double z);

    void setPosition(double x, double y);

    void setPosition(double x, double y, double z);

    void setRotation(double x, double y);

    void setScale(double x, double y);

    glm::mat4 getModel();

    void markAsDirtyState();

    void setRotationZ(double z);

    void rotateZ(double z);

    void rotateX(double x);

    void setRotationX(double x);

    void rotateY(double x);

    void setRotationY(double x);

    void rotateAround(double x, double y, double z, double angle);

    void setDirection(glm::vec3 direction);

    glm::vec3 getPosition() const;

    Transform();

    Transform(Transform& transform);

    Transform* parent = nullptr;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(.0f, .0f, .0f);
    glm::vec3 rotationAxis = glm::vec3(.0f, .0f, .0f);
    void translatePure(float d, float d1, float d2);
};


#endif //OPENGLPROJECT_TRANSFORM_H
