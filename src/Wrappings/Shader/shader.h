#pragma once

#include <filesystem>
#include <iostream>
#include <glad/glad.h>
#include <fstream>
using uint = unsigned int;
namespace fs = std::filesystem;

class Shader
{
private:
    unsigned int ID;
    void Check(const fs::path &path);
    std::string ReadFile(const fs::path &path);

public:
    Shader();
    Shader(const fs::path &path, GLenum type);
    void Init(const fs::path &path, GLenum type);
    uint GetId();
};