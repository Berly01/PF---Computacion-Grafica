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

void Camera::process_from_corners(std::vector<cv::Point2f> corners) {
    int rows = 7;
    int cols = 7;
    float squareSize = 0.25f;

    std::vector<cv::Point3f> objectPoints;
    float halfWidth = (cols - 1) * squareSize / 2.0f;
    float halfHeight = (rows - 1) * squareSize / 2.0f;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = j * squareSize - halfWidth;
            float y = i * squareSize - halfHeight;
            objectPoints.emplace_back(x, y, 0.0f);
        }
    }

    /*
    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) <<
        600, 0, 320,
        0, 600, 240,
        0, 0, 1);*/

    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) <<
        800, 0, 320,
        0, 800, 240,
        0, 0, 1);

    cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);

    //cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) <<
    //0.2413490080683482, 0.9748969769708417, 0.003429560779102727, -0.027636287913519006, -1.2621152742902424);

    cv::Mat rvec, tvec;
    cv::solvePnP(objectPoints, corners, cameraMatrix, distCoeffs, rvec, tvec);

    cv::Mat R;
    cv::Rodrigues(rvec, R);

    
    cv::Mat R_inv = R.inv(); 

   
    cv::Mat t_inv = R_inv * tvec;
    //cv::Mat t_inv = tvec;

    glm::vec3 cameraPosition(
        static_cast<float>(t_inv.at<double>(0)),
        static_cast<float>(t_inv.at<double>(1)),
        static_cast<float>(t_inv.at<double>(2)));

   

    glm::vec3 cameraUp(
        static_cast<float>(R_inv.at<double>(0, 1)),
        static_cast<float>(R_inv.at<double>(1, 1)),
        static_cast<float>(R_inv.at<double>(2, 1)));

    glm::vec3 cameraFront(
        -static_cast<float>(R_inv.at<double>(0, 2)),  
        -static_cast<float>(R_inv.at<double>(1, 2)),
        -static_cast<float>(R_inv.at<double>(2, 2)));

    position = cameraPosition;
    front = glm::normalize(cameraFront);
    up = glm::normalize(cameraUp);
    right = glm::normalize(glm::cross(front, up));


}

