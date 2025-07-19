#include "Mesh.hpp"


Mesh::Mesh(
    const std::vector<ut::Vertex3D>& verts,
    const std::vector<unsigned int>& inds,
    const ut::Material& mat) 
    : vertices(verts), indices(inds), material(mat) {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Buffer de vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ut::Vertex3D),
        vertices.data(), GL_STATIC_DRAW);

    // Buffer de indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ut::Vertex3D),
        (void*)offsetof(ut::Vertex3D, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ut::Vertex3D),
        (void*)offsetof(ut::Vertex3D, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ut::Vertex3D),
        (void*)offsetof(ut::Vertex3D, texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    if (!material.diffuseTexture.empty()) {
        diffuseTexture = std::make_unique<Texture>();
        if (!diffuseTexture->load_from_file(material.diffuseTexture)) {
            diffuseTexture.reset();
            std::cerr << "Error al cargar la textura\n";
        } 
    }
    else {
        std::cerr << "No existe una textura para cargar\n";
    }
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::draw(const Shader& shader) const {
    shader.set_vec3("material.ambient", material.ambient);
    shader.set_vec3("material.diffuse", material.diffuse);
    shader.set_vec3("material.specular", material.specular);
    shader.set_float("material.shininess", material.shininess);

    if (diffuseTexture && diffuseTexture->is_valid()) {
        diffuseTexture->bind(0);
        shader.set_int("material.diffuseMap", 0);
        shader.set_bool("material.hasTexture", true);
    }
    else {
        shader.set_bool("material.hasTexture", false);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (diffuseTexture && diffuseTexture->is_valid()) {
        diffuseTexture->unbind();
    }
}

ut::Triangle Mesh::get_triangle(size_t triangleIndex) const {
    size_t baseIndex = triangleIndex * 3;
    return ut::Triangle(
        vertices[indices[baseIndex]].position,
        vertices[indices[baseIndex + 1]].position,
        vertices[indices[baseIndex + 2]].position
    );
}
