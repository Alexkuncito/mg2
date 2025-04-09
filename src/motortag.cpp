#include "motortag.hpp"

Nodo* TMotorTAG::crearNodo(Nodo* padre, MGEntity* ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3 rotacion) {
    
    gnodos[cantnode] = make_unique<Nodo>(ent);

    Nodo* nuevoNodo = gnodos[cantnode].get();
    cantnode += 1;

    // Establecer las transformaciones del nodo
    nuevoNodo->setTraslacion(traslacion);
    nuevoNodo->setEscalado(escalado);
    nuevoNodo->setRotacion(rotacion);

    // Si se pasa un nodo padre, agregar este nuevo nodo como hijo
    if (padre != nullptr) {
        padre->agregarHijo(nuevoNodo);
    }

    // Retornar el nuevo nodo creado
    return nuevoNodo;
}


MGCamara TMotorTAG::crearCamara(Shader* shader, Camara* camara){
    MGCamara nuevaCamara(shader, camara);
    return nuevaCamara;
}

MGLuz TMotorTAG::crearLuz(Shader* shader, Luz* luz){
    MGLuz nuevaLuz(shader, luz);
    return nuevaLuz;
}

int TMotorTAG::registrarCamara(Nodo* cam){
    regCamaras.push_back(cam);
    return static_cast<int>(regCamaras.size()) - 1;
}

int TMotorTAG::registrarLuz(Nodo* luz){
    regLuces.push_back(luz);
    return static_cast<int>(regLuces.size()) - 1;
}

void TMotorTAG::activarCamara(int v){
    if (v >= 0 && v < regCamaras.size()) {
        MGCamara* camara = dynamic_cast<MGCamara*>(regCamaras[v]->getEntidad());
        if (camara) {
            camara->activar();
        }
    }
}

void TMotorTAG::activarLuz(int v){
    if (v >= 0 && v < regLuces.size()) {
        MGLuz* luz = dynamic_cast<MGLuz*>(regLuces[v]->getEntidad());
        if (luz) {
            luz->activar();
        }
    }
}

void TMotorTAG::desActivarCamara(int v){
    if (v >= 0 && v < regCamaras.size()) {
        MGCamara* camara = dynamic_cast<MGCamara*>(regCamaras[v]->getEntidad());        if (camara) {
            camara->desactivar();
        }
    }
}

void TMotorTAG::desActivarLuz(int v){
    if (v >= 0 && v < regLuces.size()) {
        MGLuz* luz = dynamic_cast<MGLuz*>(regLuces[v]->getEntidad());
        if (luz) {
            luz->desactivar();
        }
    }
}

void TMotorTAG::dibujarEscena() {
    for (auto& nodoCamara : regCamaras) {
        MGCamara* camara = dynamic_cast<MGCamara*>(nodoCamara->getEntidad());
        if (camara && camara->esActiva()) {
            camara->draw(glm::mat4(1.0f));
            break;
        }
    }

    // Activar las luces
    int luzIndex = 0;
    for (auto& nodoLuz : regLuces) {
        // Solo activamos las luces que están activadas
        MGLuz* luz = dynamic_cast<MGLuz*>(nodoLuz->getEntidad());
        if (luz && luz->esActiva()) {
            luz->draw(glm::mat4(1.0f));
        }
    }

    if (escena != nullptr) {
        escena->recorrer(glm::mat4(1.0f));
        
    }
}


MGMesh TMotorTAG::crearMalla(Shader* shader, const Fichero& fichero, 
                                              std::optional<std::reference_wrapper<const Textura>> textura, int val) {
    
    Recurso* recurso = gestorRecursos.getRecurso(fichero.getRuta(), val);
    RecursoMalla* recursoMalla = dynamic_cast<RecursoMalla*>(recurso);

    if (!recursoMalla) {
        std::unique_ptr<RecursoMalla> rcm = std::make_unique<RecursoMalla>(fichero, textura, val);
        recursoMalla = rcm.get();
        gestorRecursos.add(std::move(rcm));
    }

    Mesh* mallaMesh = recursoMalla->returnMalla();

    MGMesh malla(shader, mallaMesh);

    return malla;
}

std::vector<MGMesh> TMotorTAG::crearModeloComp(Shader* shader, const Fichero& fichero, std::optional<std::reference_wrapper<const Textura>> textura) {
    int total = fichero.numeroMeshes();  // Get the total number of meshes in the file.
    std::vector<MGMesh> mallas;  // Vector to hold the MGMesh objects.

    for(int i = 0; i < total; i++) {
        // Create a mesh for each mesh in the file.
        MGMesh malla = crearMalla(shader, fichero, textura, i);  // Assuming `i` is used as `val`.
        mallas.push_back(malla);  // Add the created mesh to the vector.
    }

    return mallas;  // Return the vector of MGMesh objects.
}


void TMotorTAG::init3D(){
    Recurso* recurso = gestorRecursos.getRecurso("Shader3D");
    RecursoShader* recursoShader = dynamic_cast<RecursoShader*>(recurso);

    if(!recursoShader){
        std::unique_ptr<RecursoShader> rsh = std::make_unique<RecursoShader>("Shader3D","../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
        recursoShader =rsh.get();
        gestorRecursos.add(std::move(rsh));
    }

    // Configuración de OpenGL para renderizado 3D
    glEnable(GL_DEPTH_TEST);                         // Activar test de profundidad
    glDepthFunc(GL_LESS);                            // Aceptar fragmentos más cercanos


    // glEnable(GL_CULL_FACE);                          // Activar el culling de caras
    // glCullFace(GL_BACK);                             // Eliminar caras traseras
    // glFrontFace(GL_CCW);                             // Triángulos en sentido antihorario son la cara frontal

    glEnable(GL_BLEND);                              // Activar blending (mezcla de transparencia)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Mezcla clásica para transparencia

    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);             // Color de fondo (se puede ajustar)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiar buffers de color y profundidad

    recursoShader->returnShader()->use();

    float fov = glm::radians(45.0f);
    glm::mat4 projection = glm::perspective(fov, (float)ventana.value().width / (float)ventana.value().height, 0.1f, 100.0f);
    recursoShader->returnShader()->setMat4("projection", projection);
}

void TMotorTAG::end3D(){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}


Shader* TMotorTAG::getShader3D(){
    Recurso* recurso = gestorRecursos.getRecurso("Shader3D");
    RecursoShader* recursoShader = dynamic_cast<RecursoShader*>(recurso);
    if(recursoShader){
        return recursoShader->returnShader();
    }
    else{
        return nullptr;
    }
}

void TMotorTAG::initWindow(int const w, int const h, char const* title){
    ventana.emplace(w,h,title);
    glfwSwapInterval(1);
    //desactivar cierre por esc
}

Window* TMotorTAG::getWindow() {
    if (ventana.has_value()) {
        return &ventana.value();
    } else {
        return nullptr;
    }
}

bool TMotorTAG::WindowIsOpen(){
    return ventana.has_value() && !ventana->shouldClose();
}

void TMotorTAG::clearBackground(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TMotorTAG::initDrawing(float r, float g, float b, float a){
    clearBackground(r,g,b,a);
}

void TMotorTAG::closeDrawing(){
    if (ventana.has_value()) {
        ventana->swapBuffers();
        glfwPollEvents();
    }
}

void TMotorTAG::DrawCube(float x, float y, float z, float width, float height, float depth, glm::vec4 color, Shader *shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::scale(model, glm::vec3(width, height, depth));

    shader->setMat4("model", model);

    Graphics3D::DrawCube(x, y, z, width, height, depth, color, shader);
}
