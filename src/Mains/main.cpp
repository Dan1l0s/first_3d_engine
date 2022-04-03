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

float WINDOW_WIDTH = 1920.0;
float WINDOW_HEIGHT = 1080.0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void init_window();
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void GetScreenSize();

Texture texture1;
Texture texture2;
Buffer cube;
Buffer light_cube;
Camera camera;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool first_mouse = true;

GLFWwindow *window;

std::vector<float> vertices = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f //
};

std::vector<unsigned int> indices = {
    0, 1, 2, // first triangle
    0, 2, 3  // second triangle
};

glm::vec3 light_color(0.9f, 0.9f, 0.9f);
// glm::vec3 object_color(1.0f, 1.0f, 1.2f);
glm::vec3 object_color(0.6f, 0.2f, 0.6f);
glm::vec3 light_pos(2.0f, 1.5f, 1.5f);

int main()
{
    init_window();
    cube.Init(vertices, indices);
    light_cube.Init(vertices, indices);

    ShaderProgram light_program;
    light_program.InitProgram();
    light_program.AddShader(WORKING_DIR / "src" / "Shaders" / "light_fragment_shader.fs", GL_FRAGMENT_SHADER);
    light_program.AddShader(WORKING_DIR / "src" / "Shaders" / "vertex_shader.vs", GL_VERTEX_SHADER);

    ShaderProgram program;
    program.InitProgram();
    program.AddShader(WORKING_DIR / "src" / "Shaders" / "vertex_shader.vs", GL_VERTEX_SHADER);
    program.AddShader(WORKING_DIR / "src" / "Shaders" / "fragment_shader.fs", GL_FRAGMENT_SHADER);

    texture1.Init(WORKING_DIR / "res" / "container2_diffuse.png");
    texture2.Init(WORKING_DIR / "res" / "container2_specular.png");

    texture1.BindToUnit(1);
    texture2.BindToUnit(2);

    program.LinkProgram();
    light_program.LinkProgram();

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
        glfwGetWindowSize(window, &width, &height);
        camera.setAspect(double(width) / double(height));

        glm::mat4 model_light = glm::mat4(1.0f);
        model_light = glm::translate(model_light, light_pos);
        model_light = glm::scale(model_light, glm::vec3(0.2f));

        light_program.Use();

        light_program.SetVec3f("lightColor", light_color);
        light_program.SetMat4fv("model", model_light);
        light_program.SetMat4fv("projection", camera.getProjectionMatrix());
        light_program.SetMat4fv("view", camera.getViewMatrix());
        light_cube.Use();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        program.Use();
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // light_pos = glm::vec3(sin((float)glfwGetTime()), 0.0f, cos((float)glfwGetTime())) * 2.0f;

        program.SetInt("material.specular", 2);
        program.SetInt("material.diffuse", 1);

        program.SetFloat("material.shininess", 64.0f);

        // light_color = glm::vec3((sin((float)glfwGetTime()) + 1) / 2, (cos((float)glfwGetTime()) + 1) / 2, 0.5f);

        glm::vec3 ambient = light_color * 0.3f;
        glm::vec3 diffuse = light_color * 0.5f;
        glm::vec3 specular = light_color * 0.7f;

        program.SetFloat("light.constant", 1.0f);
        program.SetFloat("light.linear", 0.09f);
        program.SetFloat("light.quadratic", 0.032f);

        program.SetVec3f("light.ambient", ambient);
        program.SetVec3f("light.diffuse", diffuse); // darken diffuse light a bit
        program.SetVec3f("light.direction", -0.2f, -1.0f, -0.3f);
        program.SetVec3f("light.specular", specular);
        program.SetVec3f("light.position", light_pos);

        program.SetVec3f("viewPos", camera.getPosition());
        program.SetMat4fv("model", model);
        program.SetMat4fv("projection", camera.getProjectionMatrix());
        program.SetMat4fv("view", camera.getViewMatrix());

        cube.Use();
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

    if (glfwGetKey(window, GLFW_KEY_UP))
    {
        light_pos += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN))
    {
        light_pos += glm::vec3(0.0f, -1.0f, 0.0f) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT))
    {
        light_pos += glm::vec3(-1.0f, 0.0f, 0.0f) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT))
    {
        light_pos += glm::vec3(1.0f, 0.0f, 0.0f) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
    {
        light_pos += glm::vec3(0.0f, 0.0f, 1.0f) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    {
        light_pos += glm::vec3(0.0f, 0.0f, -1.0f) * speed;
    }
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
        camera.MoveInWorld(glm::vec3(0.0f, -1.0f, 0.0f) * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE))
    {
        camera.MoveInWorld(glm::vec3(0.0f, 1.0f, 0.0f) * speed);
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

    GetScreenSize();
    // create window (empty)
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwSetWindowPos(window, 0, 25);
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

void GetScreenSize()
{
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    WINDOW_WIDTH = mode->width;
    WINDOW_HEIGHT = mode->height;
}