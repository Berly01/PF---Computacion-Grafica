#pragma once
#include <GL/glew.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "utils.hpp"

class Texture {
private:
    GLuint textureID = 0;
    int width = 0;
    int height = 0;
    int channels = 0;

public:
    Texture() = default;

    ~Texture() {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
        }
    }

    Texture(Texture&& other) noexcept
        : textureID(other.textureID), width(other.width), height(other.height), channels(other.channels) {
        other.textureID = 0;
    }

    Texture& operator=(Texture&& other) noexcept;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    bool load_from_file(const std::string& filepath);

    void load_from_mat(cv::Mat& image);

    void setup();

    void update(cv::Mat& image);

    void bind(unsigned int unit = 0) const;

    void unbind() const;

    GLuint get_ID() const { return textureID; }

    void delete_texture() const;

    bool is_valid() const { return textureID != 0; }
};