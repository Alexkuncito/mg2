#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "motortag.hpp"
#include <vector>

class Animation{
public:
    Animation(const std::string ruta, int val = 0, TMotorTAG* MTG = nullptr);
    ~Animation();

    void updateFrame(){

        if(actualFrame == framesIndex[currentFrame]){
            currentFrame++;
            if(currentFrame >= totalFrames){
                currentFrame = 0;
                actualFrame = 0;
                return;
            }
        }

        actualFrame ++;
    }

    MGMesh* getCurrentFrame() const {
        return frames[currentFrame];
    }
private:
    std::vector<MGMesh*> frames;
    std::vector<int> framesIndex;

    int totalFrames = 0;
    
    int actualFrame = 0;
    int currentFrame = 0;

    std::vector<std::string> devRutaFichero(std::string ruta) {
        std::vector<std::string> rutas;
    
        try {
            for (const auto& entry : std::filesystem::directory_iterator(ruta)) {
                if (entry.is_regular_file()) {
                    const auto& path = entry.path();
                    if (path.extension() == ".obj") {
                        rutas.push_back(path.string());
                        std::cout << "Ruta encontrada: " << path.string() << std::endl;
                    }
                }
            }
    
            // Ordena por número extraído del nombre
            std::sort(rutas.begin(), rutas.end(), [](const std::string& a, const std::string& b) {
                auto getNumber = [](const std::string& filename) -> int {
                    size_t start = filename.find_last_of('_') + 1;
                    size_t end = filename.find_last_of('.');
                    if (start == std::string::npos || end == std::string::npos || start >= end) return 0;
                    std::string numberStr = filename.substr(start, end - start);
                    return std::stoi(numberStr);
                };
                return getNumber(a) < getNumber(b);
            });
    
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error al acceder a la carpeta: " << e.what() << std::endl;
        }
    
        return rutas;
    }


std::vector<int> obtenerIndicesDisponibles(const std::string& ruta) {
    std::vector<int> indices;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(ruta)) {
            if (entry.is_regular_file()) {
                const auto& path = entry.path();

                if (path.extension() == ".obj") {
                    std::string filename = path.filename().string();

                    // Buscar un número entre el último '_' y '.'
                    size_t start = filename.find_last_of('_') + 1;
                    size_t end = filename.find_last_of('.');

                    if (start != std::string::npos && end != std::string::npos && start < end) {
                        std::string numStr = filename.substr(start, end - start);
                        try {
                            int num = std::stoi(numStr);
                            indices.push_back(num);
                        } catch (...) {
                            // Ignorar si no es un número válido
                        }
                    }
                }
            }
        }

        // Ordena los índices para mantener el orden correcto
        std::sort(indices.begin(), indices.end());

    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error al acceder a la carpeta: " << e.what() << std::endl;
    }

    return indices;
}

    
};

#endif
