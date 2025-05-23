#ifndef MODEL_ANIMATION_HPP
#define MODEL_ANIMATION_HPP

#include "motortag.hpp"
#include <vector>

struct MD{
    std::vector<MGMesh*> mallas;
};

class ModelAnimation{
public:
    ModelAnimation(bool loop = true);
    ~ModelAnimation();

    void loadFromPath(const std::string& ruta, TMotorTAG* MTG);

    void resetFrame() {
        actualFrame = 0;
        currentFrame = 0;
        finished = false;
    }

    void updateFrame(){

        if(actualFrame == framesIndex[currentFrame]){
            currentFrame++;
            if(currentFrame >= totalFrames){
                if (loop) {
                    currentFrame = 0;
                    actualFrame = 0;
                } else {
                    currentFrame = totalFrames - 1;
                    finished = true;
                }
                return;
            }
        }

        actualFrame ++;
    }

    bool haTerminado() const {
        return loop || finished;
    }

    MD getCurrentFrame() const {
        return frames[currentFrame];
    }

    int totalMallas() const {
        return (int)frames[currentFrame].mallas.size();
    }
    
private:
    std::vector<MD> frames;
    std::vector<int> framesIndex;

    int totalFrames = 0;
    
    int actualFrame = 0;
    int currentFrame = 0;

    bool loop = true;         // <--- nuevo atributo
    bool finished = false;    // <--- nuevo atributo

    std::vector<std::string> devRutaFichero(std::string ruta) {
        std::vector<std::string> rutas;
    
        try {
            for (const auto& entry : std::filesystem::directory_iterator(ruta)) {
                if (entry.is_regular_file()) {
                    const auto& path = entry.path();
                    if (path.extension() == ".obj") {
                        rutas.push_back(path.string());
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
