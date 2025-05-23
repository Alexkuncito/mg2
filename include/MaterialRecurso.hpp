#ifndef RECURSOMATERIAL_HPP
#define RECURSOMATERIAL_HPP

#include "recurso.hpp"
#include "Material.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <assimp/material.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct RecursoMaterial : public Recurso {
    private:
        TMaterial mat;
        Assimp::Importer importer;  // mantener el importer vivo
        const aiScene* scene = nullptr;
    
    public:
        RecursoMaterial(const std::string& rutaFichero, int val) : Recurso(rutaFichero, val) {
            scene = importer.ReadFile(rutaFichero,
                aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
    
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                std::cerr << "Error al cargar el archivo: " << importer.GetErrorString() << std::endl;
                mat = TMaterial(); // material por defecto
                return;
            }
    
            if (val < 0 || val >= static_cast<int>(scene->mNumMaterials)) {
                std::cerr << "Índice de material fuera de rango." << std::endl;
                mat = TMaterial(); // material por defecto
                return;
            }
    
            aiMaterial* material = scene->mMaterials[val];
            mat = TMaterial(material);
        }
    
        TMaterial returnMaterial2() const {
            return mat;
        }

        TMaterial* returnMaterial() {
            return &mat;
        }
    };
    

#endif
