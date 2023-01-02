//
// Created by gaspa on 28/12/2022.
//

#ifndef OPENGLPROJECT_TEXTUREMANAGER_H
#define OPENGLPROJECT_TEXTUREMANAGER_H

#include <string>
#include <map>
#include "glad/glad.h"


enum TextureType {
    DIRT,
    PLAYER,
    GLOW_STONE,
    WHITE_SHEEP
};



class TextureManager {
    static std::map<TextureType, GLuint> textures;
public:
    static void linkTexture(TextureType type, const char* path);

    static GLuint getTextureID(TextureType type);
};


#endif //OPENGLPROJECT_TEXTUREMANAGER_H
