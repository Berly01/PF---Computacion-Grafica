#pragma once
#include <ext/vector_float3.hpp>
#include <ext/vector_float2.hpp>

class Triangle {
private:
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;

public:
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
};
