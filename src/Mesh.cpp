#include "Mesh.hpp"

Mesh::Mesh(const Fichero& fichero, std::optional<std::reference_wrapper<const Textura>> textura, int val)
    : textura(textura) {  // Copiamos el fichero

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    TMaterial mat;

    std::cout << "Valor de Malla: " << val << std::endl;

    if (!fichero.obtenerDatos(vertices, indices, mat, val)) { 
        std::cerr << "Error: No se pudieron obtener los datos del fichero." << std::endl;
        return;
    }

    std::cout << "==============================" << std::endl;
    std::cout << "MATERIAL" << std::endl;
    std::cout << "Ambient: " 
        << std::to_string(mat.getAmbient().x) << ", " 
        << std::to_string(mat.getAmbient().y) << ", " 
        << std::to_string(mat.getAmbient().z) << std::endl;

    std::cout << "Diffuse: " 
        << std::to_string(mat.getDiffuse().x) << ", " 
        << std::to_string(mat.getDiffuse().y) << ", " 
        << std::to_string(mat.getDiffuse().z) << std::endl;

    std::cout << "Specular: " 
        << std::to_string(mat.getSpecular().x) << ", " 
        << std::to_string(mat.getSpecular().y) << ", " 
        << std::to_string(mat.getSpecular().z) << std::endl;         

    std::cout << "Shininess: " << mat.getShininess() << std::endl;

    std::cout << "==============================" << std::endl;

    material = mat;

    indexCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::draw(Shader* shader) const {
    if (!shader) {
        std::cerr << "Error: No se ha proporcionado un shader válido a la función draw()." << std::endl;
        return;
    }

    shader->use();

    material.SetMaterial(shader);
    
    if (textura) {
        textura->get().bind(); // Vincula la textura si está presente
    } else {
        glBindTexture(GL_TEXTURE_2D, 0); // Desactiva cualquier textura activa
    }

    glBindVertexArray(VAO); // Vincula el VAO de la malla
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); // Dibuja los elementos

    if (textura) {
        textura->get().unbind(); // Desvincula la textura si estaba activa
    } else {
        glBindTexture(GL_TEXTURE_2D, 0); // Asegura que ninguna textura quede activa
    }

    // Verifica errores de OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }
}
