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

class Minecraft {
    int width{};
    int height{};
    int depth{};

    glm::vec3 playerSpawn{};

    World *world;

    CameraControls *cameraControls;
    PlayerControls *playerControls;
public:
    Minecraft(int width, int height, int depth, int nbrTrees, glm::vec3 playerSpawn, GLFWwindow *window);

    void linkShader(Shader &shader);

    void render(Shader &shader);

    void processEvents(GLFWwindow *window);


    void configureMatrices(Shader &shader) const;

    Camera *camera;
    Light *light;
    GameObject *player;
    std::vector<GameObject*> toRender = std::vector<GameObject*>();
};


#endif //OPENGLPROJECT_MINECRAFT_H
