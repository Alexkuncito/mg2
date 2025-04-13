#ifndef FICHERO_HPP
#define FICHERO_HPP

#include <vector>
#include <string>
#include <Material.hpp>

struct subMesh {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    TMaterial material;
    std::string rutaTextura;
};

class Fichero {
public:
    Fichero(const std::string& rutad);

    bool obtenerDatos(std::vector<float>& outVertices, std::vector<unsigned int>& outIndices, TMaterial& outMaterial, int vk) const;
    int numeroMeshes() const;
    const std::string& getRuta() const;
    const std::string& getTextureRuta(int vl) const;

private:
    std::string ruta;
    std::vector<subMesh> val;
    unsigned int indexOffset = 0;

    void cargarMallasAssimp(const std::string& ruta);
};

#endif
