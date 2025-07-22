#pragma once
#include <memory>
#include "Shader.hpp"
#include "Mesh.hpp"
#include "OBJLoader.hpp"
#include "Camera.hpp"
#include "Background.hpp"
#include "HandGestureDetector.hpp"

#include "PatternDetector.hpp"


#include <GLFW/glfw3.h>

class Renderer {
private:
    GLFWwindow* window;
    std::unique_ptr<Shader> shader_model;
    std::unique_ptr<Shader> shader_background;
    std::unique_ptr<Mesh> mesh_model;
    std::unique_ptr<Background> background;
    std::unique_ptr<Camera> camera;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    //std::unique_ptr<HandGestureDetector> hand_detector;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    double lastX = 400.0;
    double lastY = 300.0;
    bool firstMouse = true;
    bool mouseEnabled = true;

    float modelRotationY = 0.0f;
    bool autoRotate = true;

    float rotationY = 0.0f;
    float scale = 1.0f;

    const std::string MODEL_FILE_VERT_PATH = "shaders/model_shader.vert";
    const std::string MODEL_FILE_FRAG_PATH = "shaders/model_shader.frag";
    const std::string BACKGROUND_FILE_VERT_PATH = "shaders/background_shader.vert";
    const std::string BACKGROUND_FILE_FRAG_PATH = "shaders/background_shader.frag";

public:

    bool initialize(const int& width, const int& height, const std::string& filepath);

    bool initialize_only_model(const int& width, const int& height, const std::string& filepath);

    bool initialize_only_background(const int& width, const int& height);

    void render_only_model();

    void render_only_background();

    void render();

    void cleanup();

    void set_model_scale(const float& scale);

    void show_camera_control() const;

private:

    bool load_model(const std::string& filepath);

    static void mouse_call_back(GLFWwindow* window, double xpos, double ypos) {
        auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
        renderer->handle_mouse_movement(xpos, ypos);
    }

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
        renderer->handle_mouse_scroll(yoffset);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
        renderer->handle_keyboard(key, action, mods);
    }

    void handle_mouse_movement(double xpos, double ypos) {
        if (!mouseEnabled) return;

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        auto xoffset = xpos - lastX;
        auto yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera->process_mouse_movement(xoffset, yoffset);
    }

    void handle_mouse_scroll(double yoffset) {
        camera->process_mouse_scroll(yoffset);
    }

    void handle_keyboard(int key, int action, int mods) {
        if (action == GLFW_PRESS) {
            switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
                break;
            case GLFW_KEY_TAB:

                mouseEnabled = !mouseEnabled;
                if (mouseEnabled) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    firstMouse = true;
                }
                else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                break;
            case GLFW_KEY_R:
                autoRotate = !autoRotate;
                break;
            }
        }
    }

    void process_input() {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->process_keyboard(Camera::Movement::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->process_keyboard(Camera::Movement::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->process_keyboard(Camera::Movement::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->process_keyboard(Camera::Movement::RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera->process_keyboard(Camera::Movement::UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera->process_keyboard(Camera::Movement::DOWN, deltaTime);
    }


};

