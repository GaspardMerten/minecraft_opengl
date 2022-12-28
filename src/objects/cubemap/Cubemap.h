//
// Created by Jonathan Stefanov on 28/12/2022.
//

#ifndef OPENGLPROJECT_CUBEMAP_H
#define OPENGLPROJECT_CUBEMAP_H


#include "../../utils/shader/shader/Shader.h"
#include "../game_object/GameObject.h"
#include "../camera.h"

class Cubemap : public GameObject {
private:
    GLuint cubeMapTexture{};
    void loadCubemapFace(const char *path, const unsigned int &targetFace);

public:

    // override make object
    void makeObject() override;
    void draw(Camera &camera);

    Cubemap(const std::string &pathToMesh, Shader &shader);

};


#endif //OPENGLPROJECT_CUBEMAP_H
