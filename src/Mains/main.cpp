#include <iostream>
#include "../Wrappings/Shader/shader.h"
#include "../Wrappings/ShaderProgram/shader_program.h"
#include <windows.h>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace fs = std::filesystem;

const fs::path WORKING_DIR = fs::canonical(fs::current_path() / "..");

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

float vertices[] = {
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
};
unsigned int indices[] = {
    0, 1, 2, // first triangle
};

int main()
{
    // opengl version choice
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window (empty)
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // resize draw area
    glViewport(0, 0, 800, 600);
    // custom event on resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // create vertex array
    unsigned int VAO_ID;
    glGenVertexArrays(1, &VAO_ID);

    // enable VAO_ID
    glBindVertexArray(VAO_ID);

    // buffer create
    unsigned int VBO_ID;
    glGenBuffers(1, &VBO_ID);

    // binding VBO_ID to 'window called GL_ARRAY_BUFFER'
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // create another buffer
    unsigned int EBO_ID;
    glGenBuffers(1, &EBO_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set vertex pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    ShaderProgram program;
    program.InitProgram();
    program.AddShader(WORKING_DIR / "src" / "Shaders" / "vertex_shader.vs", GL_VERTEX_SHADER);
    program.AddShader(WORKING_DIR / "src" / "Shaders" / "fragment_shader.fs", GL_FRAGMENT_SHADER);

    program.LinkProgram();

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        program.Use();

        float timeValue = glfwGetTime();
        float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
        float redValue = (std::sin(timeValue) / 3.0f) + 0.5f;
        float blueValue = (std::sin(timeValue) / 4.0f) + 0.5f;

        program.SetVec4f("ourColor", redValue, greenValue, blueValue, 1.0f);

        // drawing triangle
        // glBindVertexArray(VAO_ID);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw only lines (borders)
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // draw and fill

        // drawing rectangle
        // // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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