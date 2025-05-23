#include "../include/ModelAnimation.hpp"
#include <filesystem>
#include <algorithm>

void ModelAnimation::loadFromPath(const std::string &ruta, TMotorTAG *MTG)
{
    if (MTG == nullptr) {
        std::cerr << "Error: TMotorTAG no inicializado." << std::endl;
        return;
    }

    


    std::vector<std::string> de = devRutaFichero(ruta);
    frames.reserve(de.size());

    framesIndex = obtenerIndicesDisponibles(ruta);

    totalFrames = framesIndex.size();

    std::vector<Fichero> ficheros;
    ficheros.reserve(de.size());
    std::cout << "➡️ Entra aqui... por lo menos: " << de.size() << std::endl;

    for (const auto& path : de) {
        std::cout << "➡️ Procesando archivo: " << path << std::endl;
        MD MG2;
        MG2.mallas = MTG->getENTCOMP(path);
        std::cout << "→ Cargando modelo: " << path << std::endl;
        std::cout << "→ ¿mallas vacías?: " << MG2.mallas.empty() << std::endl;

        if (MG2.mallas.empty()) {
            if (!std::filesystem::exists(path) || std::filesystem::is_directory(path)) {
                std::cerr << "[ModelAnimation] ❌ Archivo de modelo no válido o inexistente: " << path << std::endl;
                ficheros.emplace_back("assets/MotorTAG/models/prota.obj");  // Fallback
            } else {
                ficheros.emplace_back(path);
            }


            if (!MTG) {
                std::cerr << "[ModelAnimation] ⚠️ MotorTAG es nullptr, no se pueden crear meshes." << std::endl;
                return;
            }

            std::cout<<"AquiLLEGA";
            MG2.mallas = MTG->crearModeloComp(MTG->getShader3D(), ficheros.back());
        }

        frames.push_back(MG2);
    }

}

ModelAnimation::ModelAnimation(bool loop) :currentFrame(0), actualFrame(0), totalFrames(0), loop(loop), finished(false)
{

}

ModelAnimation::~ModelAnimation() {
}
