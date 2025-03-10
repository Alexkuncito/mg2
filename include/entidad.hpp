#ifndef ENTIDAD_HPP
#define ENTIDAD_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Mesh.hpp"
#include "Camara.hpp"
#include "Luz.hpp"
#include "Shader.hpp"

// Clase base para todas las entidades
struct MGEntity
{
    public:
        MGEntity(Shader* shader);
        virtual void draw(glm::mat4 mat);
    protected:
        Shader* shader;
};

// Clase derivada para representar una malla
struct MGMesh : public MGEntity
{
    public:
        MGMesh(Shader* shader, Mesh* malla);
        void draw(glm::mat4 mat) override;
    private:
        Mesh* malla;
};

// Clase derivada para representar una cámara
struct MGCamara : public MGEntity
{
    public:
        MGCamara(Shader* shader, Camara* camara);
        void draw(glm::mat4 mat) override;
        void activar(){
            activa = true;
        };
        void desactivar(){
            activa = false;
        };
    private:
        Camara* camara;
        bool activa = false;
};

// Clase derivada para representar una luz
struct MGLuz : public MGEntity
{
    public:
        MGLuz(Shader* shader, Luz* luz);
        void draw(glm::mat4 mat) override;
        void activar(){
            activa = true;
        };
        void desactivar(){
            activa = false;
        };
    private:
        Luz* luz;
        bool activa = false;
};

#endif // ENTIDAD_HPP
