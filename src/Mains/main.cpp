#include <iostream>
#include "../Wrappings/Shader/shader.h"
#include "../Wrappings/ShaderProgram/shader_program.h"
#include "../Wrappings/Buffer/buffer.h"
#include "../Wrappings/Texture/texture.h"
#include "../Wrappings/Camera/camera.h"
#include <windows.h>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;

const fs::path WORKING_DIR = fs::canonical(fs::current_path() / "..");

const float WINDOW_WIDTH = 1600.0;
const float WINDOW_HEIGHT = 1200.0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void init_window();
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

Texture texture1;
Texture texture2;
Buffer buffer;
Camera camera;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool first_mouse = true;

GLFWwindow *window;

std::vector<float> vertices = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f

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

    texture1.Init(WORKING_DIR / "res" / "wall.png");
    texture2.Init(WORKING_DIR / "res" / "i.png");

    program.LinkProgram();

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int width, height;
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    //  render loop
    while (!glfwWindowShouldClose(window))
    {
        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);

        program.Use();

        texture1.BindToUnit(0);
        texture2.BindToUnit(1);

        program.SetInt("texture1", 0);
        program.SetInt("texture2", 1);

        glfwGetWindowSize(window, &width, &height);
        camera.setAspect(double(width) / height);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        program.SetMat4fv("model", model);

        program.SetMat4fv("projection", camera.getProjectionMatrix());
        program.SetMat4fv("view", camera.getViewMatrix());

        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float speed = 5 * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W))
    {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, 1.0f) * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, -1.0f) * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_D))
    {
        camera.MoveInLocal(glm::vec3(1.0f, 0.0f, 0.0f) * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_A))
    {
        camera.MoveInLocal(glm::vec3(-1.0f, 0.0f, 0.0f) * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_C))
    {
        camera.MoveInLocal(glm::vec3(0.0f, -1.0f, 0.0f) * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE))
    {
        camera.MoveInLocal(glm::vec3(0.0f, 1.0f, 0.0f) * speed);
    }

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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // GLAD initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    // resize draw area
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // custom event on resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.setFov(camera.getFov() - yoffset);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (first_mouse)
    {
        lastX = xpos;
        lastY = ypos;
        first_mouse = false;
    }

    float xoffset = lastX - xpos;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.12f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.RotateX(yoffset);
    camera.RotateY(xoffset);
}