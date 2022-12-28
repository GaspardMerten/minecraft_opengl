//
// Created by gaspa on 28/12/2022.
//

#ifndef OPENGLPROJECT_TEXTURE_H
#define OPENGLPROJECT_TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
public:
    GLuint textureID{};

    explicit Texture(std::string path);

    GLuint getID() const;
};


#endif //OPENGLPROJECT_TEXTURE_H
