#include "Camera.hpp"

void Camera::update_camera_vectors() {
    glm::vec3 newFront;
    newFront.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    newFront.y = static_cast<float>(sin(glm::radians(pitch)));
    newFront.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
    : position(pos), worldUp(up), yaw(yaw), pitch(pitch),
    movementSpeed(2.5f), mouseSensitivity(0.1f), zoom(45.0f) {
    update_camera_vectors();
}

void Camera::process_keyboard(
    Movement direction,
    float deltaTime) {
    float velocity = movementSpeed * deltaTime;

    switch (direction) {
    case Movement::FORWARD:
        position += front * velocity;
        break;
    case Movement::BACKWARD:
        position -= front * velocity;
        break;
    case Movement::LEFT:
        position -= right * velocity;
        break;
    case Movement::RIGHT:
        position += right * velocity;
        break;
    case Movement::UP:
        position += up * velocity;
        break;
    case Movement::DOWN:
        position -= up * velocity;
        break;
    }
}

void Camera::process_mouse_movement(
    double xoffset,
    double yoffset,
    bool constrainPitch) {

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Restringir pitch para evitar voltear la pantalla
    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    update_camera_vectors();
}

void Camera::process_mouse_scroll(double yoffset) {
    zoom -= yoffset;
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 45.0f) zoom = 45.0f;
}


