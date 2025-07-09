#include "Shader.hpp"

GLuint Shader::compile_shader(const std::string& source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error compilando shader: " << infoLog << std::endl;
    }
    return shader;
}


std::string Shader::load_shader_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexCode = load_shader_file(vertexPath);
    std::string fragmentCode = load_shader_file(fragmentPath);

    GLuint vertexShader = compile_shader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compile_shader(fragmentCode, GL_FRAGMENT_SHADER);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Error enlazando programa: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const
{
    glUseProgram(programID);
}

void Shader::set_matrix4(const std::string& name, const glm::mat4& matrix) const
{
    GLint location = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_matrix3(const std::string& name, const glm::mat3& matrix) const
{
    GLint location = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_vec3(const std::string& name, const glm::vec3& vec) const
{
    GLint location = glGetUniformLocation(programID, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void Shader::set_float(const std::string& name, float value) const
{
    GLint location = glGetUniformLocation(programID, name.c_str());
    glUniform1f(location, value);
}

void Shader::set_int(const std::string& name, int value) const
{
    GLint location = glGetUniformLocation(programID, name.c_str());
    glUniform1i(location, value);
}

void Shader::set_bool(const std::string& name, bool value) const
{
    GLint location = glGetUniformLocation(programID, name.c_str());
    glUniform1i(location, value ? 1 : 0);
}
