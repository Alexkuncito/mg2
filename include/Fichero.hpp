#ifndef FICHERO_HPP
#define FICHERO_HPP

#include <vector>
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

class Fichero {
public:
    Fichero(const std::string& ruta) {
        cargarMallasAssimp(ruta);
    }

    bool obtenerDatos(std::vector<float>& outVertices, std::vector<unsigned int>& outIndices) const {
        if (vertices.empty() || indices.empty()) return false;
        outVertices = vertices;
        outIndices = indices;
        return true;
    }

private:
    std::vector<float> vertices;       
    std::vector<unsigned int> indices;
    unsigned int indexOffset = 0; 

    void cargarMallasAssimp(const std::string& ruta) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(ruta, 
            aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Error al cargar modelo con Assimp: " << importer.GetErrorString() << std::endl;
            return;
        }

        for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
            aiMesh* mesh = scene->mMeshes[m];

            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                // Posición
                vertices.push_back(mesh->mVertices[i].x);
                vertices.push_back(mesh->mVertices[i].y);
                vertices.push_back(mesh->mVertices[i].z);

                // UVs
                if (mesh->mTextureCoords[0]) {
                    vertices.push_back(mesh->mTextureCoords[0][i].x);
                    vertices.push_back(mesh->mTextureCoords[0][i].y);
                } else {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }

                // Normales
                if (mesh->HasNormals()) {
                    vertices.push_back(mesh->mNormals[i].x);
                    vertices.push_back(mesh->mNormals[i].y);
                    vertices.push_back(mesh->mNormals[i].z);
                } else {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(face.mIndices[j] + indexOffset);
                }
            }

            indexOffset += mesh->mNumVertices;
        }
    }
};

#endif
