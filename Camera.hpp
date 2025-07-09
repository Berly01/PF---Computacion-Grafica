#pragma once
#include <ext/vector_float3.hpp>
#include <cmath>
#include <trigonometric.hpp>
#include <geometric.hpp>
#include <ext/matrix_float4x4.hpp>
#include <ext/matrix_transform.hpp>

class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    double yaw;
    double pitch;

    float movementSpeed;
    double mouseSensitivity;
    double zoom;

    void update_camera_vectors();

public:
    enum class Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f);

    glm::mat4 get_view_matrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void process_keyboard(Movement direction, float deltaTime);

    void process_mouse_movement(
        double xoffset,
        double yoffset,
        bool constrainPitch = true);

    void process_mouse_scroll(double yoffset);

    float get_zoom() const { return zoom; }
    glm::vec3 get_position() const { return position; }
    glm::vec3 get_front() const { return front; }

};

