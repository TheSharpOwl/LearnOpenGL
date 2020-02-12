#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include<vector>
#include<string>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

#include "Shader.h"
#include "Mesh.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:

	Model(std::string path)
	{
		loadModel(path);
	}

	void Draw(Shader shader);


private:

	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* Scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

