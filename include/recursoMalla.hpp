#ifndef RECURSOMALLA_HPP
#define RECURSOMALLA_HPP

#include "recurso.hpp"
#include "Mesh.hpp"
#include "Textura.hpp"
#include <vector>
#include <optional>
#include <string>

using namespace std;

struct RecursoMalla : Recurso {
    private:
        vector<float> vertices;
        vector<float> normales;
        vector<float> coordenadasTextura;
        vector<unsigned int> indices;
        optional<Mesh> malla;
        optional<Textura> textura;
    public:

        bool cargarFichero(const char* nombre) {
            Fichero fichero(nombre);

            if (!fichero.obtenerDatos(vertices, indices)) {
                cerr << "Error al cargar los datos de vértices e índices del fichero." << endl;
                return false;
            }

            normales.resize(vertices.size(), 0.0f);
            coordenadasTextura.resize(vertices.size(), 0.0f);

            malla = Mesh(fichero, textura);
            return true;
        }

        void setTextura(const Textura& nueva){
            textura = nueva;
        }

        void draw() const {
            if (malla) {
                malla->draw();
            } else {
                cerr << "ERROR: La malla no está cargada." << endl;
            }
        }


};

#endif
