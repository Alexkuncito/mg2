#ifndef FICHERO_HPP
#define FICHERO_HPP

#include <vector>
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <Material.hpp>

struct subMesh{
    std::vector<float> vertices;       
    std::vector<unsigned int> indices;
    TMaterial material;
    std::string rutaTextura;
};

class Fichero {
public:
    Fichero(const std::string& rutad) {
        ruta = rutad;
        cargarMallasAssimp(rutad);
    }


    bool obtenerDatos(std::vector<float>& outVertices, std::vector<unsigned int>& outIndices, TMaterial& outMaterial, int vk) const {
        if (vk < 0 || vk >= static_cast<int>(val.size()) || val[vk].vertices.empty() || val[vk].indices.empty()) {
            return false;
        }

        outVertices = val[vk].vertices;
        outIndices = val[vk].indices;
        outMaterial = val[vk].material;
        return true;
    }


    int numeroMeshes() const{
        return val.size();
    }

    const std::string& getRuta() const {
        return ruta;
    }

    const std::string& getTextureRuta(int vl) const {
        std::cout << "Textura: " << val[vl].rutaTextura << std::endl;
        return val[vl].rutaTextura;
    }

private:
    std::string ruta;
    // std::vector<float> vertices;       
    // std::vector<unsigned int> indices;
    std::vector<subMesh> val;
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
            subMesh vf;
            std::cout << "NUMERO DE MESHES: " << scene->mNumMeshes << std::endl;
            std::cout << "MESH " << m << std::endl;

            aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
            vf.material = TMaterial(aiMat);

            // Obtener la ruta de la textura asociada con este mesh
            if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiString texturePath;
                aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);  // Solo la primera textura de tipo difusa
                vf.rutaTextura = texturePath.C_Str();  // Guardar la ruta de la textura
            }

            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                // Posición
                vf.vertices.push_back(mesh->mVertices[i].x);
                vf.vertices.push_back(mesh->mVertices[i].y);
                vf.vertices.push_back(mesh->mVertices[i].z);

                // UVs
                if (mesh->mTextureCoords[0]) {
                    vf.vertices.push_back(mesh->mTextureCoords[0][i].x);
                    vf.vertices.push_back(mesh->mTextureCoords[0][i].y);
                } else {
                    vf.vertices.push_back(0.0f);
                    vf.vertices.push_back(0.0f);
                }

                // Normales
                if (mesh->HasNormals()) {
                    vf.vertices.push_back(mesh->mNormals[i].x);
                    vf.vertices.push_back(mesh->mNormals[i].y);
                    vf.vertices.push_back(mesh->mNormals[i].z);
                } else {
                    vf.vertices.push_back(0.0f);
                    vf.vertices.push_back(0.0f);
                    vf.vertices.push_back(0.0f);
                }
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    vf.indices.push_back(face.mIndices[j]);
                }
            }


            indexOffset += mesh->mNumVertices;
            val.push_back(vf);
        }
    }

    
};

#endif
