//
// Created by gaspa on 31/12/2022.
//

#ifndef OPENGLPROJECT_MINECRAFT_H
#define OPENGLPROJECT_MINECRAFT_H


#include "../utils/shader/shader/Shader.h"
#include "../world/World.h"
#include "../objects/camera/Camera.h"
#include "../controls/camera/CameraControls.h"
#include "../controls/player_controls/PlayerControls.h"
#include "../utils/shader/shader/Light.h"
#include "../pnj/PNJManager.h"
#include "../physics/PhysicsManager.h"

class Minecraft {
    glm::vec3 playerSpawn{};

    PlayerControls *playerControls;

    CameraControls *cameraControls;

    PNJManager *pnjManager;

    PhysicsManager *physicsManager;
public:
    Minecraft(int width, int height, int depth, int nbrTrees, int nbCircles, glm::vec3 playerSpawn, GLFWwindow *window);

    void linkShader(Shader &shader);

    void render(Shader &shader);

    void processEvents(GLFWwindow *window, Shader &shader);

    void configureMatrices(Shader &shader) const;

    void updateManagers();

    Camera *camera;
    Light *light;
    GameObject *player;
    std::vector<IGameObject*> toRender = std::vector<IGameObject*>();
    World *world;
};


#endif //OPENGLPROJECT_MINECRAFT_H
