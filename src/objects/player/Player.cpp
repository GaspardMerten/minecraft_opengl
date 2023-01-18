//
// Created by gaspa on 18/01/2023.
//

#include "Player.h"
#include "../mesh/manager/MeshManager.h"
#include "../../texture/manager/TextureManager.h"

Player::Player() {
    player = new GameObject(MeshManager::getMesh(BODY_MESH));
    leftArm = new GameObject(MeshManager::getMesh(LEFTARM_MESH), player->transform);
    rightArm = new GameObject(MeshManager::getMesh(RIGHTARM_MESH), player->transform);
    leftLeg = new GameObject(MeshManager::getMesh(LEFTLEG_MESH), player->transform);
    rightLeg = new GameObject(MeshManager::getMesh(RIGHTLEG_MESH), player->transform);

    leftLeg->transform.rotationAxis = glm::vec3(0, 1, 0);
    rightLeg->transform.rotationAxis = glm::vec3(0, 1, 0);

    rightArm->transform.rotationAxis = glm::vec3(0.37, 1.305, 0);
    rightArm->transform.rotateZ(-90);

    leftArm->transform.rotationAxis = glm::vec3(-0.37, 1.305, 0);
    leftArm->transform.rotateZ(90);


    player->setTextureID(TextureManager::getTextureID(PLAYER));
    leftArm->setTextureID(TextureManager::getTextureID(PLAYER));
    rightArm->setTextureID(TextureManager::getTextureID(PLAYER));
    leftLeg->setTextureID(TextureManager::getTextureID(PLAYER));
    rightLeg->setTextureID(TextureManager::getTextureID(PLAYER));
}

void Player::makeObject(Shader &shader) {
    player->makeObject(shader);
    leftArm->makeObject(shader);
    rightArm->makeObject(shader);
    leftLeg->makeObject(shader);
    rightLeg->makeObject(shader);
}

void Player::draw(Shader &shader) {
    player->draw(shader);
    leftArm->draw(shader);
    rightArm->draw(shader);
    leftLeg->draw(shader);
    rightLeg->draw(shader);

    float newPosValue =  player->transform.getPosition().x + player->transform.getPosition().z;


    leftArm->transform.rotateZ(-physicsData.velocity*100);
    rightArm->transform.rotateZ(physicsData.velocity*100);

    if (leftArm->transform.rotation.z > 90) {
        leftArm->transform.rotation.z = 90;
    }

    if (leftArm->transform.rotation.z < 0) {
        leftArm->transform.rotation.z = 0;
    }

    if(rightArm->transform.rotation.z < -90) {
        rightArm->transform.rotation.z = -90;
    }

    if (rightArm->transform.rotation.z > 0) {
        rightArm->transform.rotation.z = 0;
    }

    if (physicsData.velocity == 0 && physicsData.acceleration == 0) {
        leftArm->transform.rotation.z = 90;
        leftArm->transform.markAsDirtyState();
        rightArm->transform.rotation.z = -90;
        rightArm->transform.markAsDirtyState();
    }


    if (newPosValue != lastPosValue) {
        lastPosValue = newPosValue;

        if (leftLeg->transform.rotation.x > 20) {
            rotationDirection = -1;
        } else if (leftLeg->transform.rotation.x < -20) {
            rotationDirection = 1;
        }

        leftLeg->transform.rotateX( rotationDirection*10);
        rightLeg->transform.rotateX(-rotationDirection*10);
        delta = 10;
    } else {
        delta -= 1;

        if (delta <= 0) {
            leftLeg->transform.setRotationX(0);
            rightLeg->transform.setRotationX(0);
            delta = 0;
        }
    }
}

Transform* Player::getTransform()  {
    return &player->transform;
}
