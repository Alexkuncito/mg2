#ifndef TEGE_SHADER_HPP
#define TEGE_SHADER_HPP

#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();
        
        void use() const;

        void setMat4(const std::string &name, const glm::mat4 &matrix);
        void setInt(const std::string &name, int value);
        void setFloat(const std::string &name, float value);
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        int getID();

    private:
        unsigned int shaderID;
        std::string loadShaderSource(const std::string& path);
        unsigned int compileShader(unsigned int type, const std::string& source);
};

#endif
