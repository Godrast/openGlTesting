#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"


#include <iostream>
#include <vector>

#include "shader.hpp"
#include "Mesh.hpp"


namespace own
{

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
    class Model
    {
    public:
        /*  Functions   */
        Model(const char* path, int textureOffset = 0) : textureOffset(textureOffset)
        {
            loadModel(path);
        }
        void Draw(Shader  shader);
    private:

        int textureOffset;
        /*  Model Data  */
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;


        /*  Functions   */
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
            std::string typeName);

        //unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
    };


}

#endif