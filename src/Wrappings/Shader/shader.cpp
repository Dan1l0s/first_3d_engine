#include "shader.h"

Shader::Shader()
{
}

void Shader::Init(const fs::path &path, GLenum type)
{
    ID = glCreateShader(type);
    std::string source = ReadFile(path);
    const char *source1 = source.c_str();
    glShaderSource(ID, 1, &source1, NULL);
    glCompileShader(ID);
    Check(path);
}

std::string Shader::ReadFile(const fs::path &path)
{
    std::string context, tmp;
    std::ifstream fin(path);
    if (!fin.is_open())
    {
        std::cout << "ERROR: Could not open " << path << std::endl;
        return "Error";
    }
    while (getline(fin, tmp))
    {
        context.append(tmp + '\n');
    }
    return context;
}

Shader::Shader(const fs::path &path, GLenum type)
{
    Init(path, type);
}

uint Shader::GetId()
{
    return ID;
}

void Shader::Check(const fs::path &path)
{
    int success = 1;
    char infoLog[512];
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        std::cout << "Failed to compile shader: (" << path.string() << ")\n"
                  << std::string(infoLog) << std::endl;
        return;
    }
    std::cout << "Shader source file successfully compiled: (" + path.string() + ")" << std::endl;
}