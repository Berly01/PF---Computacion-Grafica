#pragma once
#include <GL/glew.h>
#include <vector>
#include "utils.hpp"
#include "Triangle.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

class Mesh {
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    std::vector<ut::Vertex3D> vertices;
    std::vector<unsigned> indices;
    std::unique_ptr<Texture> diffuseTexture;
    ut::Material material;

public:
    Mesh(
        const std::vector<ut::Vertex3D>& verts,
        const std::vector<unsigned int>& inds,
        const ut::Material& mat);

    ~Mesh();
    
    void draw(const Shader& shader) const;

    Triangle get_triangle(size_t triangleIndex) const;

    size_t get_triangle_count() const { return indices.size() / 3; }
};


