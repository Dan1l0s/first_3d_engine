#include <iostream>
#include "../Wrappings/Shader/shader.h"
#include "../Wrappings/ShaderProgram/shader_program.h"
#include "../Wrappings/Buffer/buffer.h"
#include "../Wrappings/Texture/texture.h"
#include <windows.h>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;

const fs::path WORKING_DIR = fs::canonical(fs::current_path() / "..");

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void init_window();

Texture texture1;
Texture texture2;
Buffer buffer;

GLFWwindow *window;

std::vector<float> vertices = {
    // positions          // colors           // texture coords
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
};
std::vector<unsigned int> indices = {
    0, 1, 2, // first triangle
    0, 2, 3  // second triangle
};

int main()
{
    init_window();
    buffer.Init(vertices, indices);

    ShaderProgram program;
    program.InitProgram();
    program.AddShader(WORKING_DIR / "src" / "Shaders" / "vertex_shader.vs", GL_VERTEX_SHADER);
    program.AddShader(WORKING_DIR / "src" / "Shaders" / "fragment_shader.fs", GL_FRAGMENT_SHADER);

    texture1.Init(WORKING_DIR / "res" / "wall.jpg");

    texture2.Init(WORKING_DIR / "res" / "i.png");

    program.LinkProgram();

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    //  render loop
    while (!glfwWindowShouldClose(window))
    {
        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        program.Use();

        texture1.BindToUnit(0);
        texture2.BindToUnit(1);

        program.SetInt("texture1", 0);
        program.SetInt("texture2", 1);

        float timeValue = glfwGetTime();
        float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
        float redValue = (std::sin(timeValue) / 3.0f) + 0.5f;
        float blueValue = (std::sin(timeValue) / 4.0f) + 0.5f;

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        GLint transformLoc = glGetUniformLocation(program.GetId(), "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        // drawing triangle

        glBindVertexArray(buffer.GetVAOId());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw only lines (borders)
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // draw and fill

        // swap buffers, check if there are any actions
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// custom event on resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// user input check
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void init_window()
{ // opengl version choice
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window (empty)
    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    // GLAD initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    // resize draw area
    glViewport(0, 0, 800, 600);
    // custom event on resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}