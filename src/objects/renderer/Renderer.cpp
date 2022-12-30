//
// Created by gaspa on 27/12/2022.
//

#include "Renderer.h"

void Renderer::draw(Shader &Shader, Transform &transform, Mesh &mesh, GLuint textureID) const {
    // print texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE0);

    Shader.setMatrix4("M", transform.getModel());
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.getVerticesCount());
}

void Renderer::makeObject(Shader &shader, Mesh &mesh, Transform &transform) {
    float *data = mesh.toFloatArray();
    int dataSize = mesh.getFloatArraySize();

    if (this->VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        //define VBO and VAO as active buffer and active vertex array
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    } else {
        // prinnt shit
        std::cout << "VAO already exists" << std::endl;

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }


    auto att_pos = glGetAttribLocation(shader.ID, "position");
    glEnableVertexAttribArray(att_pos);
    glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 8 * sizeof(float), (void *) 0);

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
