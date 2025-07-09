#pragma once
#include <ext/vector_float3.hpp>
#include <ext/vector_float2.hpp>
#include <string>

namespace ut {

    struct Vertex3D {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;

        Vertex3D() : position(0.0f), normal(0.0f, 0.0f, 1.0f), texCoord(0.0f) {}
        explicit Vertex3D(const glm::vec3& pos) : position(pos), normal(0.0f, 0.0f, 1.0f), texCoord(0.0f) {}
        explicit Vertex3D(const glm::vec3& pos, const glm::vec2& texCoord) : position(pos), normal(0.0f, 0.0f, 1.0f), texCoord(texCoord) {}
    };

    struct Vertex2D {
        glm::vec2 position;
        glm::vec2 texCoord;

        Vertex2D() : position(0.0f), texCoord(0.0f) {}
        explicit Vertex2D(const glm::vec2& pos) : position(pos), texCoord(0.0f) {}
        explicit Vertex2D(const glm::vec2& pos, const glm::vec2& texCoord) : position(pos), texCoord(texCoord) {}
    };

    struct Material {
        glm::vec3 ambient = glm::vec3(0.2f);
        glm::vec3 diffuse = glm::vec3(0.8f);
        glm::vec3 specular = glm::vec3(1.0f);
        float shininess = 32.0f;
        std::string diffuseTexture;
        std::string normalTexture;
        std::string specularTexture;
    };
}