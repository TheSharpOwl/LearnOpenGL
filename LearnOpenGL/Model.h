#pragma once

#include <vector>
#include <string>

//forward declarations
class Shader;
class Mesh;
class aiNode;
class aiMesh;
class aiScene;


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

