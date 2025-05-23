#ifndef ENTIDAD_HPP
#define ENTIDAD_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Mesh.hpp"
#include "Camara.hpp"
#include "Luz.hpp"
#include "Shader.hpp"
#include <memory>  // Necesario para std::shared_ptr
#include <filesystem>

namespace fs = std::filesystem;


// Clase base para todas las entidades
struct MGEntity
{
public:
    MGEntity() {};
    MGEntity(Shader* shader);  // Declaración del constructor
    virtual void draw(glm::mat4 mat, std::optional<glm::vec4> color=std::nullopt);
    virtual ~MGEntity() = default;
    MGEntity(const MGEntity& other) : shader(other.shader) {}
    virtual std::string getTipo() const { return "ESTANDAR"; }

    void setNombre(const std::string& nombre) { this->nombre = nombre; }
    
    std::string GetNombre() const { return nombre; }

    std::string crearNombre(const std::string& ruta, int val = 0) {
        fs::path pathObj(ruta);
        std::string carpetaContenedora = pathObj.parent_path().filename().string();

        std::string nombreArchivo = pathObj.stem().string();
        std::string nuevoNombre = "Entidad_" + carpetaContenedora + "_" + nombreArchivo + "_" + std::to_string(val);

        // //std::cout << "Ruta completa: " << ruta << std::endl;
        // //std::cout << "Carpeta contenedora: " << carpetaContenedora << std::endl;
        // //std::cout << "Nombre del archivo sin extensión: " << nombreArchivo << std::endl;
        // //std::cout << "Nombre generado: " << nuevoNombre << std::endl;

        setNombre(nuevoNombre);
        return nuevoNombre;
    }

    Shader* getShader() { return shader; }  

protected:
    Shader* shader;
    std::string nombre;
};

// Clase derivada para representar una malla
struct MGMesh : public MGEntity
{
public:
    MGMesh(Shader* shader, Mesh* malla); // Cambio a shared_ptr
    void draw(glm::mat4 mat, std::optional<glm::vec4> color=std::nullopt) override;
    void setMaterial(TMaterial mat){malla->setMaterial(mat);}
    virtual std::string getTipo() const { return "MESH"; }

private:
    Mesh* malla; // Cambio a shared_ptr
};

// Clase derivada para representar una cámara
struct MGCamara : public MGEntity
{
public:
    MGCamara(Shader* shader, Camara* camara); // Cambio a shared_ptr
    void draw(glm::mat4 mat, std::optional<glm::vec4> color=std::nullopt) override;
    void activar() { activa = true; }
    void desactivar() { activa = false; }
    bool esActiva() {return activa; }
    Camara* getCamera() {return camara;}
    virtual std::string getTipo() const { return "CAMARA"; }

private:
    Camara* camara; // Cambio a shared_ptr
    bool activa = false;
};

// Clase derivada para representar una luz
struct MGLuz : public MGEntity
{
public:
    MGLuz(Shader* shader, Luz* luz);
    void draw(glm::mat4 mat,std::optional<glm::vec4> color=std::nullopt) override;
    void activar() { activa = true; }
    void desactivar() { activa = false; }
    bool esActiva() {return activa; }
    virtual std::string getTipo() const { return "LUZ"; }

private:
    Luz* luz;
    bool activa = false;
};

#endif // ENTIDAD_HPP
