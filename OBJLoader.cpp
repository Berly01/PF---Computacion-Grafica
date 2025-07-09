#include "OBJLoader.hpp"

void OBJLoader::parse_face(
    const std::string& line,
    std::vector<ut::Vertex3D>& vertices,
    std::vector<unsigned>& indices)
{
    std::istringstream iss(line.substr(2)); // Saltar "f (face)"
    std::string vertex;

    std::vector<unsigned> faceIndices;

    while (iss >> vertex) {
        std::istringstream vertexStream(vertex);
        std::string posIndex;
        std::string texIndex;
        std::string normalIndex;

        // Parsear formato v/vt/vn o v//vn o v/vt o v
        std::getline(vertexStream, posIndex, '/');
        std::getline(vertexStream, texIndex, '/');
        std::getline(vertexStream, normalIndex);

        ut::Vertex3D v;

        // Posicion
        if (!posIndex.empty()) {
            int idx = std::stoi(posIndex) - 1; // OBJ usa indices basados en 1
            if (idx >= 0 && idx < tempPositions.size()) {
                v.position = tempPositions[idx];
            }
        }

        // Textura
        if (!texIndex.empty()) {
            int idx = std::stoi(texIndex) - 1;
            if (idx >= 0 && idx < tempTexCoords.size()) {
                v.texCoord = tempTexCoords[idx];
            }
        }

        // Normal
        if (!normalIndex.empty()) {
            int idx = std::stoi(normalIndex) - 1;
            if (idx >= 0 && idx < tempNormals.size()) {
                v.normal = tempNormals[idx];
            }
        }

        vertices.push_back(v);
        faceIndices.push_back(static_cast<unsigned>(vertices.size()) - 1);
    }

    // Triangular la cara si tiene mas de 3 vertices
    // Opengl solo soporta objetos de 3 caras
    for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
        indices.push_back(faceIndices[0]);
        indices.push_back(faceIndices[i]);
        indices.push_back(faceIndices[i + 1]);
    }
}

void OBJLoader::calculate_normals(std::vector<ut::Vertex3D>& vertices, const std::vector<unsigned int>& indices)
{
    for (size_t i = 0; i < indices.size(); i += 3) {
        if (i + 2 < indices.size()) {
            ut::Vertex3D& v1 = vertices[indices[i]];
            ut::Vertex3D& v2 = vertices[indices[i + 1]];
            ut::Vertex3D& v3 = vertices[indices[i + 2]];

            glm::vec3 edge1 = v2.position - v1.position;
            glm::vec3 edge2 = v3.position - v1.position;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            v1.normal += normal;
            v2.normal += normal;
            v3.normal += normal;
        }
    }

    for (auto& vertex : vertices) {
        vertex.normal = glm::normalize(vertex.normal);
    }
}

bool OBJLoader::load_MTL(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo .MTL " << filepath << std::endl;
        return false;
    }

    std::string line;
    ut::Material currentMaterial;
    std::string currentMaterialName;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "newmtl") {
            if (!currentMaterialName.empty()) {
                materials[currentMaterialName] = currentMaterial;
            }
            iss >> currentMaterialName;
            currentMaterial = ut::Material(); 
        }
        else if (prefix == "Ka") {
            iss >> currentMaterial.ambient.x >> currentMaterial.ambient.y >> currentMaterial.ambient.z;
        }
        else if (prefix == "Kd") {
            iss >> currentMaterial.diffuse.x >> currentMaterial.diffuse.y >> currentMaterial.diffuse.z;
        }
        else if (prefix == "Ks") {
            iss >> currentMaterial.specular.x >> currentMaterial.specular.y >> currentMaterial.specular.z;
        }
        else if (prefix == "Ns") {
            iss >> currentMaterial.shininess;
        }
        else if (prefix == "map_Kd") {
            iss >> currentMaterial.diffuseTexture;
        }
        else if (prefix == "map_Ks") {
            iss >> currentMaterial.specularTexture;
        }
        else if (prefix == "map_Bump" || prefix == "bump") {
            iss >> currentMaterial.normalTexture;
        }
    }

    if (!currentMaterialName.empty()) {
        materials[currentMaterialName] = currentMaterial;
    }

    file.close();
    std::cout << "Archivo MTL cargado: " << materials.size() << " materiales" << std::endl;
    return true;
}

bool OBJLoader::load_OBJ(
    const std::string& filepath,
    std::vector<ut::Vertex3D>& vertices,
    std::vector<unsigned int>& indices,
    ut::Material& material) {

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filepath << std::endl;
        return false;
    }

    std::string line;
    bool hasNormals = false;
    std::string currentMaterial;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            // Vértice
            float x;
            float y;
            float z;
            iss >> x >> y >> z;
            tempPositions.emplace_back(x, y, z);
        }
        else if (prefix == "vn") {
            // Normal
            float x;
            float y;
            float z;
            iss >> x >> y >> z;
            tempNormals.emplace_back(x, y, z);
            hasNormals = true;
        }
        else if (prefix == "vt") {
            // Coordenada de textura
            float u;
            float v;
            iss >> u >> v;
            tempTexCoords.emplace_back(u, v);
        }
        else if (prefix == "f") {
            // Cara
            parse_face(line, vertices, indices);
        }
        else if (prefix == "mtllib") {
            // Cargar archivo de materiales
            std::string mtlFile;
            iss >> mtlFile;

            // Construir ruta completa del archivo MTL
            std::string mtlPath = filepath.substr(0, filepath.find_last_of("/\\") + 1) + mtlFile;
            load_MTL(mtlPath);
        }
        else if (prefix == "usemtl") {
            // Usar material
            iss >> currentMaterial;
        }
    }

    // Si no hay normales en el archivo, calcularlas
    if (!hasNormals) {
        calculate_normals(vertices, indices);
    }

    // Asignar material si existe
    if (!currentMaterial.empty() && materials.find(currentMaterial) != materials.end()) {
        material = materials[currentMaterial];
    }

    file.close();

    std::cout << "Modelo cargado: " << vertices.size() << " vertices, "
        << indices.size() / 3 << " triangulos" << std::endl;

    return true;
}

