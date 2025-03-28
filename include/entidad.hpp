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

// Clase base para todas las entidades
struct MGEntity
{
public:
    MGEntity() {};
    MGEntity(Shader* shader);  // Declaración del constructor
    virtual void draw(glm::mat4 mat);
    MGEntity(const MGEntity& other) : shader(other.shader) {}
    virtual std::string getTipo() const { return "ESTANDAR"; }

protected:
    Shader* shader; // Cambio a shared_ptr
};

// Clase derivada para representar una malla
struct MGMesh : public MGEntity
{
public:
    MGMesh(Shader* shader, Mesh* malla); // Cambio a shared_ptr
    void draw(glm::mat4 mat) override;
    virtual std::string getTipo() const { return "MESH"; }

private:
    Mesh* malla; // Cambio a shared_ptr
};

// Clase derivada para representar una cámara
struct MGCamara : public MGEntity
{
public:
    MGCamara(Shader* shader, Camara* camara); // Cambio a shared_ptr
    void draw(glm::mat4 mat) override;
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
    void draw(glm::mat4 mat) override;
    void activar() { activa = true; }
    void desactivar() { activa = false; }
    bool esActiva() {return activa; }
    virtual std::string getTipo() const { return "LUZ"; }

private:
    Luz* luz;
    bool activa = false;
};

#endif // ENTIDAD_HPP
