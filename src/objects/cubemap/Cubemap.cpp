#include "Cubemap.h"
#include "../../utils/shader/shader/Shader.h"
#include "../game_object/GameObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../camera/Camera.h"

#include <map>

void Cubemap::loadCubemapFace(const char * path, const GLenum& targetFace)
{
    int imWidth, imHeight, imNrChannels;
    unsigned char* data = stbi_load(path, &imWidth, &imHeight, &imNrChannels, 0);
    if (data)
    {
        glTexImage2D(targetFace, 0, GL_RGB, imWidth, imHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(targetFace);
    }
    else {
        std::cout << "Failed to Load texture" <<
                  std::endl;
        const char* reason = stbi_failure_reason();
        std::cout << reason << std::endl;
    }
    stbi_image_free(data);
}

void Cubemap::makeObject() {
    GameObject::makeObject();


    glGenTextures(1, &cubeMapTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

    // texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //stbi_set_flip_vertically_on_load(true);

    std::string pathToCubeMap = R"(resources/)";

    std::map<std::string, GLenum> facesToLoad = {
            {pathToCubeMap + "front.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_X},
            {pathToCubeMap + "top.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
            {pathToCubeMap + "left.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
            {pathToCubeMap + "back.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
            {pathToCubeMap + "bottom.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
            {pathToCubeMap + "right.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_Z},
    };
    //load the six faces
    for (std::pair<std::string, GLenum> pair : facesToLoad) {
        loadCubemapFace(pair.first.c_str(), pair.second);
    }

}

void Cubemap::draw(Camera &camera) {
    shader.use();
    shader.setMatrix4("V", camera.getViewMatrix());
    shader.setMatrix4("P", camera.getProjectionMatrix());
    shader.setInteger("cubemapTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMapTexture);
    GameObject::draw();
    glDepthFunc(GL_LESS);

}

Cubemap::Cubemap(const std::string &pathToMesh, Shader &shader) : GameObject(pathToMesh, shader) {
    makeObject();
}