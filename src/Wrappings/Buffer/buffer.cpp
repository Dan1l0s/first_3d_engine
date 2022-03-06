#include "buffer.h"

Buffer::Buffer()
{
}

void Buffer::Init(const std::vector<float> &vertices, const std::vector<uint> &indices)
{
    glGenVertexArrays(1, &VAO_ID);
    glGenBuffers(1, &VBO_ID);
    glGenBuffers(1, &EBO_ID);

    glBindVertexArray(VAO_ID);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

uint Buffer::GetVAOId()
{
    return VAO_ID;
}
uint Buffer::GetVBOId()
{
    return VBO_ID;
}
uint Buffer::GetEBOId()
{
    return EBO_ID;
}