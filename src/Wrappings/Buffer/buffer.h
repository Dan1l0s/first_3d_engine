#pragma once
#include <glad/glad.h>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
using uint = unsigned int;

class Buffer
{
private:
    uint VAO_ID, VBO_ID, EBO_ID;

public:
    Buffer();
    void Init(const std::vector<float> &vertices, const std::vector<uint> &indices);
    void SetFloatAttribute(GLuint index, GLint size, GLboolean normalized, unsigned int stride, unsigned int offset);
    uint GetVAOId();
    uint GetVBOId();
    uint GetEBOId();
    void Use();
};