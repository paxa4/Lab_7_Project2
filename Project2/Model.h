#ifndef MODEL_H
#define MODEL_H
#include <GL/GL.h>
#include "GLFW/glfw3.h"
#include <glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "shader_loader.h"
#include <string>
#include <vector>
#include <iostream>

class Model {
public:
    std::vector<Mesh> meshes;
    std::string directory;

    Model(const std::string& path) {
        loadModel(path);
    }

    void Draw1(GLuint shader_program, glm::mat4 transformOX1, glm::mat4 transformOX2, glm::mat4 transformOX3) {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            switch (i) {
            case 1:
                modelMatrix = transformOX1;
                break;
            case 2:
                modelMatrix = transformOX2;
                break;
            case 3:
                modelMatrix = transformOX3;
                break;
            default:
                modelMatrix = glm::mat4(1.0f);
            }
            glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

  
            glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
            glUniformMatrix3fv(glGetUniformLocation(shader_program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

            meshes[i].Draw(shader_program);
        }
    }

private:
    void loadModel(const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR: IMPORT FAILED � " << importer.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            if (mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            vertices.push_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }
        return Mesh(vertices, indices);
    }
};
#endif