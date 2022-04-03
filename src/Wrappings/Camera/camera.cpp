#include "camera.h"

Camera::Camera()
{
    _direction = glm::vec3(0.0f, 0.0f, -1.0f);
    _position = glm::vec3(0.0f, 0.0f, 0.0f);
    _cameraRight = glm::normalize(glm::cross(_direction, _worldUp));
    _cameraUp = glm::normalize(glm::cross(_cameraRight, _direction));
    _fov = DEFAULT_CAMERA_FOV;
    _aspect = DEFAULT_CAMERA_ASPECT;
    updateViewMatrix();
    updateProjectionMatrix();
}

void Camera::MoveInWorld(glm::vec3 delta)
{
    _position += delta;
    updateViewMatrix();
}

void Camera::MoveInLocal(glm::vec3 delta)
{
    _position += _cameraRight * delta.x;
    _position += _cameraUp * delta.y;
    _position += _direction * delta.z;
    updateViewMatrix();
}

void Camera::LookAt()
{
    _lookAtMatrix[0][0] = _cameraRight.x;
    _lookAtMatrix[1][0] = _cameraRight.y;
    _lookAtMatrix[2][0] = _cameraRight.z;
    _lookAtMatrix[0][1] = _cameraUp.x;
    _lookAtMatrix[1][1] = _cameraUp.y;
    _lookAtMatrix[2][1] = _cameraUp.z;
    _lookAtMatrix[0][2] = -_direction.x;
    _lookAtMatrix[1][2] = -_direction.y;
    _lookAtMatrix[2][2] = -_direction.z;
}

void Camera::setPosition(glm::vec3 _position)
{
    this->_position = _position;
    updateViewMatrix();
}

void Camera::setDirection(glm::vec3 _direction)
{
    this->_direction = _direction;
    _cameraRight = glm::normalize(glm::cross(_direction, _worldUp));
    _cameraUp = glm::normalize(glm::cross(_cameraRight, _direction));
    updateViewMatrix();
}

glm::mat4 Camera::getViewMatrix()
{
    return _viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return _projectionMatrix;
}

void Camera::setFov(float fov)
{
    if (fov > 179)
        fov = 179;
    if (fov < 1)
        fov = 1;
    _fov = fov;
    updateProjectionMatrix();
}

void Camera::updateViewMatrix()
{
    glm::mat4 view = glm::mat4(1.0f);
    LookAt();
    view = view * _lookAtMatrix;
    view = glm::translate(view, -_position);
    _viewMatrix = view;
}

void Camera::updateProjectionMatrix()
{
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(_fov), _aspect, 0.1f, 1000.0f);
    _projectionMatrix = projection;
}

float Camera::getFov()
{
    return _fov;
}

glm::vec3 Camera::getPosition()
{
    return _position;
}

glm::vec3 Camera::getDirection()
{
    return _direction;
}

void Camera::RotateX(float angle)
{
    angle *= -1;
    float pitch = 90.0f - glm::angle(_direction, _worldUp) * 180 / 3.14159265358979323846;
    float new_pitch = pitch + angle;
    if (new_pitch > 89.0f)
    {
        new_pitch = 89;
    }
    if (new_pitch < -89.0f)
    {
        new_pitch = -89;
    }
    setDirection(glm::rotate(_direction, glm::radians(new_pitch - pitch), _cameraRight));
}

void Camera::RotateY(float angle)
{
    angle *= -1;
    setDirection(glm::rotate(_direction, glm::radians(angle), _worldUp));
}

float Camera::getAspect()
{
    return _aspect;
}

void Camera::setAspect(float aspect)
{
    _aspect = aspect;
}