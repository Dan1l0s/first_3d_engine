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
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

    SetFloatAttribute(0, 3, GL_FALSE, 5, 0);
    // SetFloatAttribute(1, 3, GL_FALSE, 8, 3);
    SetFloatAttribute(1, 2, GL_FALSE, 5, 3);
}

void Buffer::SetFloatAttribute(GLuint index, GLint size, GLboolean normalized, unsigned int stride, unsigned int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glVertexAttribPointer(index, size, GL_FLOAT, normalized, stride * sizeof(float), (void *)(offset * sizeof(float)));
    glEnableVertexAttribArray(index);
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

void Buffer::Use()
{
    glBindVertexArray(VAO_ID);
}