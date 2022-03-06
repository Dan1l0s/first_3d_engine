#pragma once
#include <stb_image.h>
#include <filesystem>
#include <iostream>
#include <glad/glad.h>

namespace fs = std::filesystem;
using uint = unsigned int;

class Texture
{
private:
    uint ID;
    void Check(unsigned char *data, int width, int height, int nrChannels);

public:
    Texture();
    void Init(const fs::path &path);
    void Generate(const fs::path &path);
    void BindToUnit(int number);
    uint GetId();
};