#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Camera
{
private:
    glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 _position;
    glm::vec3 _direction;
    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;
    glm::mat4 _lookAtMatrix = glm::mat4(1.0f);
    float _fov;
    glm::vec3 _cameraRight;
    glm::vec3 _cameraUp;

public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 direction);
    Camera(glm::vec3 pos);
    void setPosition(glm::vec3 pos);
    void setDirection(glm::vec3 direction);
    void setFov(float fov);
    float getFov();
    glm::vec3 getPosition();
    glm::vec3 getDirection();
    void MoveInWorld(glm::vec3 delta);
    void MoveInLocal(glm::vec3 delta);
    void RotateX(float angle);
    void RotateY(float angle);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void updateViewMatrix();
    void updateProjectionMatrix();

    void LookAt();
};