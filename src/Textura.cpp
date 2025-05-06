#define STB_IMAGE_IMPLEMENTATION
#include "../extern/stb_image.h"
#include "Textura.hpp"
#include <filesystem>


Textura::Textura(const std::string& path) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        std::cout << "Textura cargada correctamente: " << path
                  << " (" << width << "x" << height << ", " << nrChannels << " canales)\n";
    } else {
        std::cout << "Directorio actual: " << std::filesystem::current_path() << std::endl;
        std::cerr << "Error: No se pudo cargar la textura en " << path << std::endl;
    }

    stbi_image_free(data);
}

void Textura::bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Textura::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

Textura::~Textura() {
    glDeleteTextures(1, &ID);
}
