#pragma once

#include <vector>
#include <string>

//forward declarations
class Shader;
class Mesh;
class aiNode;
class aiMesh;
class aiScene;
struct Texture;
class aiMaterial;
class aiTextureType;

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:

	Model(char* path)
	{
		loadModel(path);
	}

	void Draw(Shader shader);


private:

	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* Scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

