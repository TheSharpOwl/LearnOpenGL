#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<string>
#include<vector>

class Shader;


struct Vertex
{
	glm::vec3 Postion;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	// tangent
		glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};
struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	/*Functions*/
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indecies, std::vector<Texture> textures);
	void Draw(Shader shader);

	/*render data*/ // I put then public for the planet and rock instancing example
	unsigned int VAO, VBO, EBO;
private:
	/*functions*/
	void setupMesh();
};

