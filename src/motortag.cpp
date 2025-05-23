#include "../include/motortag.hpp"


//
// ======================= CONSTRUCTOR =======================
//

TMotorTAG::TMotorTAG() : escena(nullptr), cantnode(0) {
    // gnodos[cantnode] = std::make_unique<Nodo>();
    // escena = gnodos[cantnode].get();
    // cantnode += 1;
    gnodos.reserve(10000);
    entidades.reserve(50);
    camv.reserve(2);
    regCamaras.reserve(10);
    regLuces.reserve(10);

    gnodos.push_back( std::make_unique<Nodo>());
    escena = gnodos.back().get();
    // std::cout<<"INIT 2D"<<std::endl;
    // init2D();
    // std::cout<<"INIT 3D"<<std::endl;
    // init3D();
}

//
// ======================= CREACIÓN DE NODOS Y ENTIDADES =======================
//

Nodo* TMotorTAG::crearNodo(Nodo* padre, MGEntity* ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3 rotacion) {
    gnodos.push_back(std::make_unique<Nodo>(ent));

    Nodo* nuevoNodo = gnodos.back().get();
    // cantnode += 1;

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
    // int id = static_cast<int>(entidades.size());
    entidades.push_back(std::move(nuevaCamara));
    return camaraRef;
}

Camara TMotorTAG::createCAM(float fovy, glm::vec3 position, glm::vec3 target, glm::vec3 up, float speed, float sensitivity) {
    glm::vec3 front = glm::normalize(target - position);
    Camara cam(position, front, up, speed, sensitivity);
    camv.push_back(cam);
    return cam;
}


MGLuz* TMotorTAG::crearLuz(Shader* shader, Luz* luz) {
    auto nuevaLuz = std::make_unique<MGLuz>(shader, luz);
    MGLuz* luzRef = nuevaLuz.get();
    // int id = static_cast<int>(entidades.size());
    entidades.push_back(std::move(nuevaLuz));
    return luzRef;
}

MGMesh* TMotorTAG::crearMalla(Shader* shader, const Fichero& fichero, std::optional<std::reference_wrapper<const Textura>> textura, int val) {
    MGEntity* entidad = gestorEntidades.getEntidad(fichero.getRuta(), val);
    MGMesh* mallaRef = dynamic_cast<MGMesh*>(entidad);

    if(!mallaRef) {
        Recurso* recurso = gestorRecursos.getRecurso(fichero.getRuta(), val);
        RecursoMalla* recursoMalla = dynamic_cast<RecursoMalla*>(recurso);
    
        if (!recursoMalla) {
            std::unique_ptr<RecursoMalla> rcm = std::make_unique<RecursoMalla>(fichero, textura, val);
            recursoMalla = rcm.get();
            gestorRecursos.add(std::move(rcm));
        }
    
        Mesh* mallaMesh = recursoMalla->returnMalla();
        std::unique_ptr<MGMesh> malla = std::make_unique<MGMesh>(shader, mallaMesh);
        malla->setNombre(malla->crearNombre(fichero.getRuta(), val));

        mallaRef = malla.get();
        gestorEntidades.add(std::move(malla));
    }
    else{
        //std::cout << "Malla ya existe: " << mallaRef->GetNombre() << std::endl;
    }

    return mallaRef;
}

std::vector<MGMesh*> TMotorTAG::crearModeloComp(Shader* shader, const Fichero& fichero) {
    int total = fichero.numeroMeshes();
    std::vector<MGMesh*> mallas(total);  // Vector de tamaño fijo

    for (int i = 0; i < total; ++i) {
        std::optional<std::reference_wrapper<const Textura>> tex2 = devText(fichero, i);
        MGMesh* malla = crearMalla(shader, fichero, tex2, i);
        mallas[i] = malla;
    }

    return mallas;
}



MGEntity* TMotorTAG::crearEntidadVacia(Shader* shader) {
    auto entidadVacia = std::make_unique<MGEntity>(shader);
    MGEntity* entidadRef = entidadVacia.get();
    // int id = static_cast<int>(entidades.size());
    entidades.push_back(std::move(entidadVacia));
    return entidadRef;
}


ParticleGenerator3D*  TMotorTAG::crearGenParticulas(Textura* textura, unsigned int amount, float particleScale, float horizontal, float vertical, float profundidad, std::string form, std::string particleForm){
    auto generadorParticulas = std::make_unique<ParticleGenerator3D>(getShader3DPart(), textura, amount, particleScale, horizontal, vertical, profundidad, form, particleForm);
    ParticleGenerator3D* generadorRef = generadorParticulas.get();
    generadoresParticulas.push_back(std::move(generadorParticulas));
    std::cout << "Generador de partículas creado" << std::endl;
    return generadorRef;
}

void TMotorTAG::updateParticleGen(ParticleGenerator3D* gen, glm::vec3 position, glm::vec3 velocity, unsigned int newParticles, glm::vec3 offset)
{
    if (gen) {
        auto shader = getShader3DPart();
        gen->Update(position, velocity, newParticles, offset);
        
    }
}

void TMotorTAG::resetEscena(){
    //std::cout << "Reset de escena" << std::endl;
    // 1. Eliminar todos los hijos del nodo raíz (escena)
    auto hijos = escena->getHijos();
    for (auto* hijo : hijos) {
        escena->borrarHijo(hijo);
    }

    // 2. Limpiar registros de nodos y entidades (excepto escena)
    gnodos.clear();
    camv.clear();
    regCamaras.clear();
    regLuces.clear();
    entidades.clear();

     // 3. Volver a registrar el nodo raíz escena como único nodo
     gnodos.push_back(std::make_unique<Nodo>()); // Crear nuevo nodo raíz
     escena = gnodos.back().get();
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
    if (generadoresParticulas
        .size() > 0) {
        getShader3DPart()->use();

        for (auto& generador : generadoresParticulas) {
            if(generador)
                generador->Draw(camaraP.getViewMatrix());
        }
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
    glm::mat4 projection = glm::perspective(fov, (float)ventana->width / (float)ventana->height, 0.1f, 100.0f);
    recursoShader->returnShader()->setMat4("projection", projection);


    MGCamara* entCAMARA = crearCamara(getShader3D(), &camaraP);

    MGLuz* entLUZ = crearLuz(getShader3D(), &luzP);

    glm::vec3 tras{0.0f, 0.0f, 0.0f};
    glm::vec3 esc{1.0f, 1.0f, 1.0f};
    glm::vec3 rot{0.0f, 0.0f, 0.0f};

    // Nodo para cámara y luz
    Nodo* nodocamara = crearNodo(getRaiz(), entCAMARA, tras, esc, rot);
    Nodo* nodoluz = crearNodo(getRaiz(), entLUZ, tras, esc, rot);

    // Registrar cámara y luz
    registrarCamara(nodocamara);
    registrarLuz(nodoluz);

    Recurso* recursopart = gestorRecursos.getRecurso("Shader3DPart");
    RecursoShader* recursoShaderPart = dynamic_cast<RecursoShader*>(recursopart);
    if (!recursoShaderPart) {
        auto rsh = std::make_unique<RecursoShader>("Shader3DPart", "../shaders/vertex_particle3d.glsl", "../shaders/fragment_particle3d.glsl");
        recursoShaderPart = rsh.get();
        gestorRecursos.add(std::move(rsh));
    }
}

void TMotorTAG::end3D(){
    // glDisable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);
}

Shader* TMotorTAG::getShader3D(){
    Recurso* recurso = gestorRecursos.getRecurso("Shader3D");
    RecursoShader* recursoShader = dynamic_cast<RecursoShader*>(recurso);
    return recursoShader ? recursoShader->returnShader() : nullptr;
}

Shader* TMotorTAG::getShader3DTrans(){
    Recurso* recurso = gestorRecursos.getRecurso("Shader3DTrans");
    RecursoShader* recursoShader = dynamic_cast<RecursoShader*>(recurso);
    return recursoShader ? recursoShader->returnShader() : nullptr;
}

Shader* TMotorTAG::getShader3DPart(){
    Recurso* recurso = gestorRecursos.getRecurso("Shader3DPart");
    RecursoShader* recursoShader = dynamic_cast<RecursoShader*>(recurso);
    return recursoShader ? recursoShader->returnShader() : nullptr;
}

//
// ======================= CONTROL DE VENTANA =======================
//

void TMotorTAG::initWindow(int const w, int const h, char const* title){
    ventana.emplace(w, h, title);
    glfwSwapInterval(1); // Sincronización vertical
    init2D();
    Graphics2D::LoadFont("assets/MotorTAG/fonts/jack-of-gears.regular.ttf", 40);
    init3D();
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

void TMotorTAG::clearBackground(float r, float g, float b, float a) {
    if (!glIsEnabled(GL_DEPTH_TEST)) {
        glEnable(GL_DEPTH_TEST);  // Asegúrate de que el depth test esté habilitado
    }

    glClearColor(r, g, b, a);  // Configurar el color de fondo

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Limpia los buffers de color y profundidad

    glViewport(0, 0, ventana->width, ventana->height);  // Ajusta las dimensiones a tu contexto, ejemplo: windowWidth, windowHeight
}


void TMotorTAG::initDrawing(float r, float g, float b, float a){
    //clearBackground(r, g, b, a);
}

void TMotorTAG::closeDrawing(){
    // if (ventana) {
    //     ventana->swapBuffers();
    //     glfwPollEvents();
    // }
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
    //Graphics2D::init2D(getWindow()->width, getWindow()->height);

    /////////////////////////////////////////////////


    Recurso* recurso = gestorRecursos.getRecurso("Shader2D");
    RecursoShader* recursoShader = dynamic_cast<RecursoShader*>(recurso);


    if (!recursoShader) {
        auto rsh = std::make_unique<RecursoShader>("Shader2D", "../shaders/vertex_2d.glsl", "../shaders/fragment_2d.glsl");
        recursoShader = rsh.get();
        gestorRecursos.add(std::move(rsh));
    }


    /////////////////////////////////////////////////


    /////////////////////////////////////////////////


    Recurso* recurso2 = gestorRecursos.getRecurso("ShaderText2D");
    RecursoShader* recursoShader2 = dynamic_cast<RecursoShader*>(recurso2);


    if (!recursoShader2) {
        auto rsh = std::make_unique<RecursoShader>("ShaderText2D", "../shaders/vertex_text2d.glsl", "../shaders/fragment_text2d.glsl");
        recursoShader2 = rsh.get();
        gestorRecursos.add(std::move(rsh));
    }


    /////////////////////////////////////////////////

    /////////////////////////////////////////////////


    Recurso* recurso3 = gestorRecursos.getRecurso("shaderTexture2D");
    RecursoShader* recursoShader3 = dynamic_cast<RecursoShader*>(recurso3);


    if (!recursoShader3) {
        auto rsh = std::make_unique<RecursoShader>("shaderTexture2D", "../shaders/vertex_texture2d.glsl", "../shaders/fragment_texture2d.glsl");
        recursoShader3 = rsh.get();
        gestorRecursos.add(std::move(rsh));
    }


    /////////////////////////////////////////////////


    Graphics2D::init2D(getWindow()->width, getWindow()->height, recursoShader->returnShader(), recursoShader2->returnShader(), recursoShader3->returnShader());

}

void TMotorTAG::end2D(){
    //std::cout << "Fin de 2D" << std::endl;
}

int TMotorTAG::MideTexto(std::string text, float scale){
    int ancho = Graphics2D::MeasureText(text, scale);
    return ancho;
}


// Función que carga todos los archivos OBJ de la carpeta assets y sus subdirectorios
void TMotorTAG::cargaTotalOBJ(const std::string& path) {
    // Recorrer todos los archivos .obj dentro de la carpeta y sus subdirectorios
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
        // Verificar si el archivo tiene extensión .obj
        if (entry.is_regular_file() && entry.path().extension() == ".obj") {
            std::string archivoObj = entry.path().string();
            
            // Crear un objeto Fichero para el archivo .obj
            Fichero fichero(archivoObj);
            //std::cout << "RUTA DE CARGA: " << archivoObj << std::endl;
            for(int i=0; i<fichero.numeroMeshes();i++)
            {
                std::unique_ptr<RecursoMalla> rcm = std::make_unique<RecursoMalla>(fichero, devText(fichero, i), i);
                RecursoMalla* recursoMalla = rcm.get();
                gestorRecursos.add(std::move(rcm));
                
            }
            
        }
    }
}