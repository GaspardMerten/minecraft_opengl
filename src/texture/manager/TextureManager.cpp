//
// Created by gaspa on 28/12/2022.
//

#include <iostream>
#include "TextureManager.h"
#include "../texture/Texture.h"

void TextureManager::linkTexture(TextureType type, const char* path) {
    textures[type] = Texture(path).getID();
}

GLuint TextureManager::getTextureID(TextureType type) {
    if (textures.find(type) != textures.end()) {
        return textures[type];
    } else {
        std::cout << "Error: texture not found - " << type << std::endl;
        return -1;
    }
}

std::map<TextureType, GLuint> TextureManager::textures = std::map<TextureType, GLuint>();