#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Mesh.hpp"
#include "Camara.hpp"
#include "Luz.hpp"
#include "Shader.hpp"

struct MGEntity
{
    public:
        MGEntity(Shader* shader) : shader(shader) {}
        virtual void draw(glm::mat4 mat) {
            
        }
    protected:
        Shader* shader;
};

struct MGMesh : MGEntity
{
    public:
        MGMesh(Shader* shader, Mesh* malla) : MGEntity(shader), malla(malla) {}
        void draw(glm::mat4 mat) override {
            shader->setMat4("model", mat);
            malla->draw();
        }
    private:
        Mesh* malla;
};

struct MGCamara : MGEntity
{
    public:
        MGCamara(Shader* shader, Camara* camara) : MGEntity(shader), camara(camara) {}
        void draw(glm::mat4 mat) override {
            glm::mat4 view = camara->getViewMatrix();
            shader->setMat4("view", view);
        }
    private:
        Camara* camara;
};

struct MGLuz : MGEntity
{
    public:
        MGLuz(Shader* shader, Luz* luz) : MGEntity(shader), luz(luz) {}
        void draw(glm::mat4 mat) override {
            luz->aplicar(*shader);
        }
    private:
        Luz* luz;
};
