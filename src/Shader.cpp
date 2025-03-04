#include "Shader.hpp"
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertex);
    glAttachShader(shaderID, fragment);
    glLinkProgram(shaderID);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "Error en enlace de shaders: " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(shaderID);
}

void Shader::use() const {
    glUseProgram(shaderID);
}

int Shader::getID(){
    return shaderID;
}

std::string Shader::loadShaderSource(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de shader: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error en compilación de shader (" 
                    << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") 
                    << "): " << infoLog << std::endl;
    }

    return shader;
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix) {
    GLuint location = glGetUniformLocation(shaderID, name.c_str());
    if (location == -1) {
        std::cerr << "Uniform no encontrado: " << name << std::endl;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setInt(const std::string &name, int value) {
    GLuint location = glGetUniformLocation(shaderID, name.c_str());
    if (location == -1) {
        std::cerr << "Uniform no encontrado: " << name << std::endl;
    }
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) {
    GLuint location = glGetUniformLocation(shaderID, name.c_str());
    if (location == -1) {
        std::cerr << "Uniform no encontrado: " << name << std::endl;
    }
    glUniform1f(location, value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    GLuint location = glGetUniformLocation(shaderID, name.c_str());
    if (location == -1) {
        std::cerr << "Uniform no encontrado: " << name << std::endl;
    }
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    GLuint location = glGetUniformLocation(shaderID, name.c_str());
    if (location == -1) {
        std::cerr << "Uniform no encontrado: " << name << std::endl;
    }
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    GLuint location = glGetUniformLocation(shaderID, name.c_str());
    if (location == -1) {
        std::cerr << "Uniform no encontrado: " << name << std::endl;
    }
    glUniform4fv(location, 1, &value[0]);
}
