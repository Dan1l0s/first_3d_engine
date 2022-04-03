#pragma once
#include "../Shader/shader.h"
#include <glad/glad.h>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
namespace fs = std::filesystem;
using uint = unsigned int;

class ShaderProgram
{
private:
    std::vector<uint> shaders;
    uint ID;

public:
    ShaderProgram();
    void InitProgram();
    void LinkProgram();
    void AddShader(const fs::path &path, GLenum type);
    int PosOf(const std::string &name);
    void Use();
    uint GetId();
    void SetFloat(const std::string &uniform_name, float value);
    void SetInt(const std::string &uniform_name, int value);
    void SetBool(const std::string &uniform_name, int value);
    void SetVec4f(const std::string &uniform_name, float x, float y, float z, float w);
    void SetVec4f(const std::string &uniform_name, glm::vec4 value);
    void SetMat4fv(const std::string &uniform_name, glm::mat4 value);
    void SetVec3f(const std::string &uniform_name, float x, float y, float z);
    void SetVec3f(const std::string &uniform_name, glm::vec3 vec);
    void SetVec3i(const std::string &uniform_name, int x, int y, int z);
};