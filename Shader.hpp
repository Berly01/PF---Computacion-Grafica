#pragma once
#include <GL/glew.h>
#include <gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
private:
    GLuint programID;

    GLuint compile_shader(const std::string& source, GLenum type);

    std::string load_shader_file(const std::string& path);


public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);

    ~Shader() { glDeleteProgram(programID); }

    void use() const;

    void set_matrix4(const std::string& name, const glm::mat4& matrix) const;

    void set_matrix3(const std::string& name, const glm::mat3& matrix) const;

    void set_vec3(const std::string& name, const glm::vec3& vec) const;

    void set_float(const std::string& name, float value) const;

    void set_int(const std::string& name, int value) const;

    void set_bool(const std::string& name, bool value) const;
};

