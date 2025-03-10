#include "gestorrecursos.hpp"
#include <filesystem>


// Método para obtener un recurso por su nombre
Recurso* TGestorRecursos::getRecurso(const char* nombre) {
    // Buscar el recurso en el vector
    for (auto rec : recursos) {
        if (strcmp(rec->GetNombre(), nombre) == 0) {
            return rec;  // Si lo encontramos, lo retornamos
        }
    }
    return nullptr;
}

// Método para añadir un recurso al gestor
void TGestorRecursos::add(Recurso* recurso) {
    recursos.push_back(recurso);  // Añadimos el recurso al vector
}

void TGestorRecursos::ImprimirRecursos() const {
    for (auto rec : recursos) {
        // Intentamos hacer cast a las clases derivadas (RecursoMaterial, etc.)
        if (dynamic_cast<RecursoMaterial*>(rec)) {
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
            RecursoMaterial* recursoMaterial = new RecursoMaterial();
            
            // Intentar cargar el archivo de material
            if (recursoMaterial->cargarFichero(entry.path().string())) {
                // Si el archivo se carga correctamente, agregarlo al gestor de recursos
                add(recursoMaterial);
                std::cout << "Material cargado: " << recursoMaterial->GetNombre() << std::endl;
            } else {
                delete recursoMaterial;  // Si hubo un error al cargar, liberar la memoria
            }
        }
    }
}

RecursoMalla* TGestorRecursos::getRecursoMalla(const Fichero& fichero, std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt,std::optional<TMaterial> material = std::nullopt){
    for (auto rec : recursos) {
        // Intentamos hacer cast a RecursoMalla
        RecursoMalla* recursoMalla = dynamic_cast<RecursoMalla*>(rec);
        if (recursoMalla) {
            // Comparamos por referencia al fichero (puedes adaptar si quieres comparar rutas)
            if (&(recursoMalla->getFichero()) == &fichero) {
                return recursoMalla;  // Encontrado
            }
        }
    }

    // Si no se encontró, creamos uno nuevo
    RecursoMalla* nuevaMalla = new RecursoMalla(fichero, textura, material);
    
    // Añadimos al gestor
    add(nuevaMalla);

    // Devolvemos la referencia al nuevo recurso
    return nuevaMalla;
}
