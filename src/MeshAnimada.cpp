#include "MeshAnimada.hpp"
#include <filesystem>
#include <algorithm>

Animation::Animation(const std::string ruta, int val, TMotorTAG* MTG)
    : currentFrame(0)
{

    if (MTG == nullptr) {
        std::cerr << "Error: TMotorTAG no inicializado." << std::endl;
        return;
    }



    std::vector<std::string> de = devRutaFichero(ruta);
    frames.reserve(de.size());

    std::vector<Fichero> ficheros;
    ficheros.reserve(de.size());

    framesIndex = obtenerIndicesDisponibles(ruta);
    for (const auto& index : framesIndex) {
        std::cout << "Index: " << index << std::endl;
    }

    totalFrames = framesIndex.size();

    for(int i = 0; i < de.size(); i++){
        std::cout << de[i] << std::endl;
        ficheros.push_back(Fichero(de[i]));
        MGMesh* MGF = MTG->crearMalla(MTG->getShader3D(), ficheros[i], nullopt, val); //AQUI VA EN LA TEXTURA LO DE DEVTEXT
        frames.push_back(MGF);
    }

    std::cout << "NUMERO DE ANIMACIONES: " << frames.size() << std::endl;

}

Animation::~Animation() {
}
