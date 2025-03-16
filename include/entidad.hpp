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
    MGEntity(std::shared_ptr<Shader> shader);  // Declaración del constructor
    virtual void draw(glm::mat4 mat);
    MGEntity(const std::shared_ptr<MGEntity>& other) : shader(other->shader) {}

protected:
    std::shared_ptr<Shader> shader; // Cambio a shared_ptr
};

// Clase derivada para representar una malla
struct MGMesh : public MGEntity
{
public:
    MGMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> malla); // Cambio a shared_ptr
    void draw(glm::mat4 mat) override;
private:
    std::shared_ptr<Mesh> malla; // Cambio a shared_ptr
};

// Clase derivada para representar una cámara
struct MGCamara : public MGEntity
{
public:
    MGCamara(std::shared_ptr<Shader> shader, std::shared_ptr<Camara> camara); // Cambio a shared_ptr
    void draw(glm::mat4 mat) override;
    void activar() { activa = true; }
    void desactivar() { activa = false; }
    bool esActiva() {return activa; }
    std::shared_ptr<Camara> getCamera() {return camara;}
private:
    std::shared_ptr<Camara> camara; // Cambio a shared_ptr
    bool activa = false;
};

// Clase derivada para representar una luz
struct MGLuz : public MGEntity
{
public:
    MGLuz(std::shared_ptr<Shader> shader, std::shared_ptr<Luz> luz); // Cambio a shared_ptr
    void draw(glm::mat4 mat) override;
    void activar() { activa = true; }
    void desactivar() { activa = false; }
    bool esActiva() {return activa; }
private:
    std::shared_ptr<Luz> luz; // Cambio a shared_ptr
    bool activa = false;
};

#endif // ENTIDAD_HPP
