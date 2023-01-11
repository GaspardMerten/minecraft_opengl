//
// Created by gaspa on 27/12/2022.
//

#include "Renderer.h"
#include "../../texture/manager/TextureManager.h"

void Renderer::draw(Shader &shader, Transform &transform, Mesh *mesh, GLuint textureID) const {
    // print texture
    if (shader.withTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(glGetUniformLocation(shader.ID, "tex"), 0);
    }

    shader.setMatrix4("M", transform.getModel());

    if (textureID == TextureManager::getTextureID(TextureType::WATER)) {
        shader.setFloat("opacity", .7);
        shader.setBool("isWater", true);
    } else {
        shader.setBool("isWater", false);
        shader.setFloat("opacity", 1);
    }

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh->getVerticesCount());
}

void Renderer::makeObject(Shader &shader, Mesh *mesh, Transform &transform) {
    float *data = mesh->toFloatArray();
    int dataSize = mesh->getFloatArraySize();

    if (this->VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        //define VBO and VAO as active buffer and active vertex array
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    } else {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }


    auto att_pos = glGetAttribLocation(shader.ID, "position");
    glEnableVertexAttribArray(att_pos);
    glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 8 * sizeof(float), (void *) nullptr);

    if (shader.withTexture) {
        auto att_tex = glGetAttribLocation(shader.ID, "tex_coord");
        glEnableVertexAttribArray(att_tex);
        glVertexAttribPointer(att_tex, 2, GL_FLOAT, false, 8 * sizeof(float), (void *) (3 * sizeof(float)));

        auto att_col = glGetAttribLocation(shader.ID, "normal");
        glEnableVertexAttribArray(att_col);
        glVertexAttribPointer(att_col, 3, GL_FLOAT, false, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    }


    //desactive the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::makeObject(Shader shader, Mesh *pMesh, Transform transform, Renderer renderer) {
    VAO = renderer.VAO;
    VBO = renderer.VBO;
    makeObject(shader, pMesh, transform);
}
