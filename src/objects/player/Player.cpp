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



    if (newPosValue != lastPosValue) {
        std::cout << "Player moved" << std::endl;
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
