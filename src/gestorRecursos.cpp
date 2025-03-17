#include "gestorrecursos.hpp"
#include <filesystem>

// Método para obtener un recurso por su nombre
std::shared_ptr<Recurso> TGestorRecursos::getRecurso(const std::string& ruta) {
    // Generar el nombre del recurso a partir de la ruta
    std::string nombreGenerado = Recurso(ruta).crearNombre(ruta); // Crear el nombre del recurso basado en la ruta

    // Buscar el recurso en el vector
    for (const auto& rec : recursos) {
        // Comparar el nombre generado con el nombre del recurso almacenado
        if (strcmp(rec->GetNombre(), nombreGenerado.c_str()) == 0) {
            return rec;  // Si lo encontramos, lo retornamos
        }
    }

    
    return nullptr; // Si no lo encontramos, retornamos nullptr
}

// Método para añadir un recurso al gestor
void TGestorRecursos::add(std::shared_ptr<Recurso> recurso) {
    recursos.push_back(recurso);  // Añadimos el recurso al vector
}

void TGestorRecursos::ImprimirRecursos() const {
    for (const auto& rec : recursos) {
        // Intentamos hacer cast a las clases derivadas (RecursoMaterial, etc.)
        if (std::dynamic_pointer_cast<RecursoMaterial>(rec)) {
            std::cout << "Recurso: Material -> " << rec->GetNombre() << std::endl;
        }
        else {
            std::cout << "Recurso: Desconocido -> " << rec->GetNombre() << std::endl;
        }
    }
}

void TGestorRecursos::cargarMateriales(){
    namespace fs = std::filesystem;
    
    // Ruta donde se encuentran los archivos de materiales
    std::string ruta = "../material/";

    // Verificar si el directorio existe
    if (!fs::exists(ruta)) {
        std::cerr << "Error: El directorio " << ruta << " no existe." << std::endl;
        return;
    }

    // Recorrer todos los archivos en el directorio
    for (const auto& entry : fs::directory_iterator(ruta)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {  // Asumiendo que los archivos de material tienen extensión .txt
            // Crear una nueva instancia de RecursoMaterial
            auto recursoMaterial = std::make_shared<RecursoMaterial>();
            
            // Intentar cargar el archivo de material
            if (recursoMaterial->cargarFichero(entry.path().string())) {
                // Si el archivo se carga correctamente, agregarlo al gestor de recursos
                add(recursoMaterial);
                std::cout << "Material cargado: " << recursoMaterial->GetNombre() << std::endl;
            }
        }
    }
}
