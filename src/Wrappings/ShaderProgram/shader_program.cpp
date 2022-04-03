#include "shader_program.h"

ShaderProgram::ShaderProgram()
{
}

void ShaderProgram::InitProgram()
{
    ID = glCreateProgram();
}

void ShaderProgram::AddShader(const fs::path &path, GLenum type)
{
    Shader shader(path, type);
    shaders.push_back(shader.GetId());
    glAttachShader(ID, shader.GetId());
}

void ShaderProgram::LinkProgram()
{
    glLinkProgram(ID);
    for (const auto &shader : shaders)
    {
        glDeleteShader(shader);
    }
    shaders.clear();
}

int ShaderProgram::PosOf(const std::string &name)
{
    return glGetUniformLocation(ID, name.c_str());
}

void ShaderProgram::Use()
{
    glUseProgram(ID);
}

uint ShaderProgram::GetId()
{
    return ID;
}

void ShaderProgram::SetFloat(const std::string &uniform_name, float value)
{
    Use();
    glUniform1f(PosOf(uniform_name), value);
}
void ShaderProgram::SetInt(const std::string &uniform_name, int value)
{
    Use();
    glUniform1i(PosOf(uniform_name), value);
}
void ShaderProgram::SetVec4f(const std::string &uniform_name, float x, float y, float z, float w)
{
    Use();
    glUniform4f(PosOf(uniform_name), x, y, z, w);
}
void ShaderProgram::SetBool(const std::string &uniform_name, int value)
{
    Use();
    glUniform1i(PosOf(uniform_name), value);
}
void ShaderProgram::SetVec4f(const std::string &uniform_name, glm::vec4 value)
{
    Use();
    glUniform4f(PosOf(uniform_name), value.x, value.y, value.z, value.w);
}

void ShaderProgram::SetMat4fv(const std::string &uniform_name, glm::mat4 value)
{
    Use();
    glUniformMatrix4fv(PosOf(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetVec3f(const std::string &uniform_name, float x, float y, float z)
{
    Use();
    glUniform3f(PosOf(uniform_name), x, y, z);
}
void ShaderProgram::SetVec3f(const std::string &uniform_name, glm::vec3 vec)
{
    Use();
    glUniform3f(PosOf(uniform_name), vec.x, vec.y, vec.z);
}
void ShaderProgram::SetVec3i(const std::string &uniform_name, int x, int y, int z)
{
    Use();
    glUniform3i(PosOf(uniform_name), x, y, z);
}