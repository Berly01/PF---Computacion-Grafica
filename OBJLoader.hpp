#pragma once
#include <geometric.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "utils.hpp"


class OBJLoader {
private:
    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;
    std::unordered_map<std::string, ut::Material> materials;

    void parse_face(
        const std::string& line,
        std::vector<ut::Vertex3D>& vertices,
        std::vector<unsigned int>& indices);

    void calculate_normals(
        std::vector<ut::Vertex3D>& vertices,
        const std::vector<unsigned int>& indices);

    bool load_MTL(const std::string& filepath);

public:
    bool load_OBJ(
        const std::string& filepath,
        std::vector<ut::Vertex3D>& vertices,
        std::vector<unsigned int>& indices,
        ut::Material& material);
};

