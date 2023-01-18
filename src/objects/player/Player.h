//
// Created by gaspa on 18/01/2023.
//

#ifndef OPENGLPROJECT_PLAYER_H
#define OPENGLPROJECT_PLAYER_H


#include "../game_object/GameObject.h"

class Player : public IGameObject {
    GameObject* leftArm;
    GameObject* rightArm;
    GameObject* leftLeg;
    GameObject* rightLeg;

public:
    void makeObject(Shader &shader) override;

    void draw(Shader &shader) override;

    Transform* getTransform() override;

    Player();

    GameObject* player;

    float  lastPosValue = 0.f;
    int rotationDirection = 1;


    int delta = 0;
};


#endif //OPENGLPROJECT_PLAYER_H
