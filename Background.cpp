#include "Background.hpp"

Background::Background(const int& width, const int& height, const int& index)
    : windowWidth(width), windowHeight(height) {
    
    cap.open(index);
    if (!cap.isOpened()) {
        std::cerr << "Error: No se pudo abrir la camara" << std::endl;
        exit(-1);
    }

    vertices = {
        ut::Vertex2D(glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),        
        ut::Vertex2D(glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
        ut::Vertex2D(glm::vec2(1.0f,  1.0f), glm::vec2(1.0f, 1.0f)),
        ut::Vertex2D(glm::vec2(-1.0f,  1.0f), glm::vec2(0.0f, 1.0f))
    };

    indices = {
        0, 1, 2,   
        2, 3, 0    
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ut::Vertex2D), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    texture.setup();
}

Background::~Background() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Background::draw(const Shader& shader) {

    update_texture();

    shader.use();

    // Activar textura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.get_ID());

    // Renderizar quad
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Background::update_texture() {
    cap >> frame;
    if (frame.empty()) return;

    texture.update(frame);
}
