#include "motortag.hpp"

//
// ======================= CONSTRUCTOR =======================
//

TMotorTAG::TMotorTAG() : escena(nullptr), cantnode(0) {
    gnodos[cantnode] = std::make_unique<Nodo>();
    escena = gnodos[cantnode].get();
    cantnode += 1;
}

//
// ======================= CREACIÓN DE NODOS Y ENTIDADES =======================
//

Nodo* TMotorTAG::crearNodo(Nodo* padre, MGEntity* ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3 rotacion) {
    gnodos[cantnode] = std::make_unique<Nodo>(ent);
    Nodo* nuevoNodo = gnodos[cantnode].get();
    cantnode += 1;

    nuevoNodo->setTraslacion(traslacion);
    nuevoNodo->setEscalado(escalado);
    nuevoNodo->setRotacion(rotacion);

    if (padre != nullptr) {
        padre->agregarHijo(nuevoNodo);
    }

    return nuevoNodo;
}

MGCamara* TMotorTAG::crearCamara(Shader* shader, Camara* camara) {
    auto nuevaCamara = std::make_unique<MGCamara>(shader, camara);
    MGCamara* camaraRef = nuevaCamara.get();
    int id = static_cast<int>(entidades.size());
    entidades[id] = std::move(nuevaCamara);
    return camaraRef;
}

MGLuz* TMotorTAG::crearLuz(Shader* shader, Luz* luz) {
    auto nuevaLuz = std::make_unique<MGLuz>(shader, luz);
    MGLuz* luzRef = nuevaLuz.get();
    int id = static_cast<int>(entidades.size());
    entidades[id] = std::move(nuevaLuz);
    return luzRef;
}

MGMesh* TMotorTAG::crearMalla(Shader* shader, const Fichero& fichero, std::optional<std::reference_wrapper<const Textura>> textura, int val) {
    Recurso* recurso = gestorRecursos.getRecurso(fichero.getRuta(), val);
    RecursoMalla* recursoMalla = dynamic_cast<RecursoMalla*>(recurso);

    if (!recursoMalla) {
        std::unique_ptr<RecursoMalla> rcm = std::make_unique<RecursoMalla>(fichero, textura, val);
        recursoMalla = rcm.get();
        gestorRecursos.add(std::move(rcm));
    }

    Mesh* mallaMesh = recursoMalla->returnMalla();
    auto malla = std::make_unique<MGMesh>(shader, mallaMesh);
    MGMesh* mallaRef = malla.get();

    int id = static_cast<int>(entidades.size());
    entidades[id] = std::move(malla);

    return mallaRef;
}

std::vector<MGMesh*> TMotorTAG::crearModeloComp(Shader* shader, const Fichero& fichero, std::optional<std::reference_wrapper<const Textura>> textura) {
    int total = fichero.numeroMeshes();
    std::vector<MGMesh*> mallas;

    for (int i = 0; i < total; i++) {
        MGMesh* malla = crearMalla(shader, fichero, textura, i);
        mallas.push_back(malla);
    }

    return mallas;
}

MGEntity* TMotorTAG::crearEntidadVacia(Shader* shader) {
    auto entidadVacia = std::make_unique<MGEntity>(shader);
    MGEntity* entidadRef = entidadVacia.get();
    int id = static_cast<int>(entidades.size());
    entidades[id] = std::move(entidadVacia);
    return entidadRef;
}

//
// ======================= REGISTRO Y CONTROL DE CÁMARAS Y LUCES =======================
//

int TMotorTAG::registrarCamara(Nodo* cam){
    regCamaras.push_back(cam);
    return static_cast<int>(regCamaras.size()) - 1;
}

int TMotorTAG::registrarLuz(Nodo* luz){
    regLuces.push_back(luz);
    return static_cast<int>(regLuces.size()) - 1;
}

void TMotorTAG::activarCamara(int v) {
    if (v >= 0) {
        size_t index = static_cast<size_t>(v);
        if (index < regCamaras.size()) {
            MGCamara* camara = dynamic_cast<MGCamara*>(regCamaras[index]->getEntidad());
            if (camara) camara->activar();
        }
    }
}

void TMotorTAG::activarLuz(int v) {
    if (v >= 0) {
        size_t index = static_cast<size_t>(v);
        if (index < regLuces.size()) {
            MGLuz* luz = dynamic_cast<MGLuz*>(regLuces[index]->getEntidad());
            if (luz) luz->activar();
        }
    }
}

void TMotorTAG::desActivarCamara(int v) {
    if (v >= 0) {
        size_t index = static_cast<size_t>(v);
        if (index < regCamaras.size()) {
            MGCamara* camara = dynamic_cast<MGCamara*>(regCamaras[index]->getEntidad());
            if (camara) camara->desactivar();
        }
    }
}

void TMotorTAG::desActivarLuz(int v) {
    if (v >= 0) {
        size_t index = static_cast<size_t>(v);
        if (index < regLuces.size()) {
            MGLuz* luz = dynamic_cast<MGLuz*>(regLuces[index]->getEntidad());
            if (luz) luz->desactivar();
        }
    }
}


Camara* TMotorTAG::getCamaraActiva() {
    if (!regCamaras.empty()) {
        MGCamara* mgCamara = dynamic_cast<MGCamara*>(regCamaras[0]->getEntidad());
        return mgCamara ? mgCamara->getCamera() : nullptr;
    }
    return nullptr;
}

//
// ======================= DIBUJADO DE ESCENA =======================
//

void TMotorTAG::dibujarEscena() {
    for (auto& nodoCamara : regCamaras) {
        MGCamara* camara = dynamic_cast<MGCamara*>(nodoCamara->getEntidad());
        if (camara && camara->esActiva()) {
            camara->draw(glm::mat4(1.0f));
            break;
        }
    }

    for (auto& nodoLuz : regLuces) {
        MGLuz* luz = dynamic_cast<MGLuz*>(nodoLuz->getEntidad());
        if (luz && luz->esActiva()) {
            luz->draw(glm::mat4(1.0f));
        }
    }

    if (escena) {
        escena->recorrer(glm::mat4(1.0f));
    }
}

void TMotorTAG::pinta() {
    gestorRecursos.ImprimirRecursos();
    if (escena) {
        imprimirArbol(escena, 0);
    }
}

//
// ======================= INICIALIZACIÓN Y FINALIZACIÓN DE 3D =======================
//

void TMotorTAG::init3D(){
    Recurso* recurso = gestorRecursos.getRecurso("Shader3D");
    RecursoShader* recursoShader = dynamic_cast<RecursoShader*>(recurso);

    if (!recursoShader) {
        auto rsh = std::make_unique<RecursoShader>("Shader3D", "../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
        recursoShader = rsh.get();
        gestorRecursos.add(std::move(rsh));
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    recursoShader->returnShader()->use();

    float fov = glm::radians(45.0f);
    glm::mat4 projection = glm::perspective(fov, (float)ventana->width / ventana->height, 0.1f, 100.0f);
    recursoShader->returnShader()->setMat4("projection", projection);
}

void TMotorTAG::end3D(){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

Shader* TMotorTAG::getShader3D(){
    Recurso* recurso = gestorRecursos.getRecurso("Shader3D");
    RecursoShader* recursoShader = dynamic_cast<RecursoShader*>(recurso);
    return recursoShader ? recursoShader->returnShader() : nullptr;
}

//
// ======================= CONTROL DE VENTANA =======================
//

void TMotorTAG::initWindow(int const w, int const h, char const* title){
    ventana.emplace(w, h, title);
    glfwSwapInterval(1); // Sincronización vertical
}

void TMotorTAG::closeWindow(){
    if (ventana) {
        ventana->close();
        ventana.reset();
    }
}

Window* TMotorTAG::getWindow() {
    return ventana ? &ventana.value() : nullptr;
}

bool TMotorTAG::WindowIsOpen(){
    return ventana && !ventana->shouldClose();
}

void TMotorTAG::clearBackground(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TMotorTAG::initDrawing(float r, float g, float b, float a){
    clearBackground(r, g, b, a);
}

void TMotorTAG::closeDrawing(){
    if (ventana) {
        ventana->swapBuffers();
        glfwPollEvents();
    }
}

//
// ======================= DIBUJO 2D / 3D =======================
//

void TMotorTAG::DrawCube(float x, float y, float z, float width, float height, float depth, glm::vec4 color, Shader *shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::scale(model, glm::vec3(width, height, depth));
    shader->setMat4("model", model);

    Graphics3D::DrawCube(x, y, z, width, height, depth, color, shader);
}

void TMotorTAG::DrawRectangle(float x, float y, float width, float height, glm::vec4 color) {
    Graphics2D::DrawRectangle(x, y, width, height, color);
}

void TMotorTAG::DrawCircle(float x, float y, float radius, glm::vec4 color) {
    Graphics2D::DrawCircle(x, y, radius, color);
}

void TMotorTAG::LoadFont(const std::string& fontPath, unsigned int fontSize) {
    Graphics2D::LoadFont(fontPath, fontSize);
}

void TMotorTAG::DrawText(std::string text, float x, float y, float scale, glm::vec3 color) {
    Graphics2D::DrawText(text, x, y, scale, color);
}

void TMotorTAG::DrawTexture(float x, float y, float width, float height, const Textura& texture) {
    Graphics2D::DrawTexture(x, y, width, height, texture);
}

//
// ======================= SISTEMA DE DIBUJO 2D =======================
//

void TMotorTAG::init2D(){
    Graphics2D::init2D();
}

void TMotorTAG::end2D(){
    std::cout << "Fin de 2D" << std::endl;
}
