#include "Texture.hpp"


Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other) {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
        }
        textureID = other.textureID;
        width = other.width;
        height = other.height;
        channels = other.channels;
        other.textureID = 0;
    }
    return *this;
}

bool Texture::load_from_file(const std::string& filepath)
{
    cv::Mat image = cv::imread(filepath, cv::IMREAD_COLOR);

    if (image.empty()) {
        std::cerr << "Error: No se pudo cargar la imagen " << filepath << std::endl;
        return false;
    }

    // OpenCV carga en formato BGR, convertir a RGB
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

    // Voltear la imagen verticalmente (OpenGL usa coordenadas 'Y' invertidas)
    cv::flip(image, image, 0);

    width = image.cols;
    height = image.rows;
    channels = image.channels();

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "Textura cargada: " << filepath << " (" << width << "x" << height << ", "
        << channels << " canales)" << std::endl;

    return true;
}

void Texture::load_from_mat(cv::Mat& image) {

    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

    cv::flip(image, image, 0);

    width = image.cols;
    height = image.rows;
    channels = image.channels();

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image.data);
    
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setup() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::update(cv::Mat& image) {
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

    cv::flip(image, image, 0);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::delete_texture() const {
    if (textureID != 0) glDeleteTextures(1, &textureID);
}
