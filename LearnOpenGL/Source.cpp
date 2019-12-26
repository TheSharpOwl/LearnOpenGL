//always make sure to include glad before glfw
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<math.h>

#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define _USE_MATH_DEFINES

void initGLFW()
{
	if (!glfwInit())
		exit(-1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window);
void MouseCallBack(GLFWwindow* window, double xpos, double ypos);
void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

//Function for reading a shader file given the path

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") : " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path : " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
float Mix;

float DeltaTime;//automatically zero
float LastFrame;//automatically zero
bool FirstMouse = true;
//Camera Part
Camera OurCamera(glm::vec3(0.f,0.f,3.f));
float LastX = 400, LastY = 300;

int main()
{
	
	//make sure that we're using version 3 fir both minor and major
	initGLFW();

	//Generate the Window Object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);//we should make this directly after creating the window
	glfwSetCursorPosCallback(window, MouseCallBack);//register our mouse function
	glfwSetScrollCallback(window, ScrollCallBack);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//capture the cursor and don't let it go out of the window

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//initialize GLAD before any OpenGL call but 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glViewport(0, 0, 800, 600);//parameters : x,y for the lower left corner of the window, last 2 are width and height

	/*
	*Note that processed coordinates in OpenGL are between -1 and 1 
	*so we effectively map from the range (-1 to 1) to (0, 800) and (0, 600). 
	*/

	//register so the function so that it gets called everytime we resize the window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);

	Shader LightingShader("ColorsVertex.glsl", "ColorsFragment.glsl");
	Shader LampShader("LampVertex.glsl", "LampFragment.glsl");

	/*
	Each vertex attribute takes its data from memory managed by a VBO and which VBO it takes its data from
	(you can have multiple VBOs) is determined by the VBO currently bound to GL_ARRAY_BUFFER
	 when calling glVertexAttribPointer. Since the previously defined VBO is still bound
	before calling glVertexAttribPointer vertex attribute 0 is now associated with its vertex data.
	*/

	//6 suquares, 2 triangles for each to draw a cube
	//the second 3 positions in each line are for the surface normals

	float Vertices[] = 
	{
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	glm::vec3 CubePositions[] = 
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//VBO is the same for both, the light and the cubes
	unsigned int VBO, CubeVAO;

	//The cube part
	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindVertexArray(CubeVAO);

	//position attribute
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//the normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//the texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//------- The light part -------
	unsigned int LightVAO;
	glGenVertexArrays(1, &LightVAO);
	glBindVertexArray(LightVAO);


	//find the VBO (no need to fill it, it contains all the needed information)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	const int Width = 800, Height = 600;
	glm::vec3 LightPos(2.0f, 0.f, 2.0f);

	//texture things
	std::string woodenTexturePath = "resources/textures/container2.png";
	unsigned int diffuseMap = loadTexture(woodenTexturePath.c_str());

	std::string steelTexturePath = "resources/textures/container2_specular.png";
	unsigned int specularMap = loadTexture(steelTexturePath.c_str());

	double time = 0.0;

	LightingShader.Use();
	LightingShader.SetInt("material.diffuse", 0);
	LightingShader.SetInt("material.specular", 1);

	// light properties
	LightingShader.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	LightingShader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	LightingShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
	LightingShader.SetFloat("material.shininess", 64.0f);

	LightingShader.SetVec3("light.position", OurCamera.Position);
	LightingShader.SetVec3("light.direction", OurCamera.Front);
	LightingShader.SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
	LightingShader.SetFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		float CurrentFrame = float(glfwGetTime());
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;

		processInput(window);//definition is up there =)

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use the shader before setting up the uniforms' values !!!
		LightingShader.Use();
		LightingShader.SetVec3("light.position", LightPos);
		LightingShader.SetVec3("viewPos", OurCamera.Position);


		// material properties
		
		//Coordinate system :
		glm::mat4 projection;
		glm::mat4 view = OurCamera.GetViewMatrix();
		projection = glm::perspective(glm::radians(OurCamera.Zoom), float(Width) / float(Height), 0.1f, 100.f);
		LightingShader.SetMat4("projection", projection);
		LightingShader.SetMat4("view", view);

		glm::mat4 model;

		//bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		//render the cube
		glBindVertexArray(CubeVAO);
		//world transformation and showing all the cubes
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, CubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			LightingShader.SetMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// draw the lamp
		LampShader.Use();
		LampShader.SetMat4("projection", projection);
		LampShader.SetMat4("view", view);

		const double  Pi = 3.14159265358979323846;
		time += 0.02f;
		LightPos.x = sqrt(8.0) * cos(time) + 2.0;
		LightPos.z = sqrt(8.0) * sin(time) + 2.0;

		model = glm::mat4(1.f);
		model = glm::translate(model, LightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		LampShader.SetMat4("model", model);

		glBindVertexArray(LightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwPollEvents();//checks if any events are triggered like input


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &CubeVAO);
	glDeleteVertexArrays(1, &LightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
//if we press the escape key set the 'should close' to true so the window will close
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//if it's not pressed, it returns GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);

	//Buttons for camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		OurCamera.ProcessKeyboard(FORWARD, DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		OurCamera.ProcessKeyboard(BACKWARD, DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		OurCamera.ProcessKeyboard(LEFT, DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		OurCamera.ProcessKeyboard(RIGHT, DeltaTime);

	//buttons for textures
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		Mix += 0.001f;
		if (Mix >= 1.f)
			Mix = 1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		Mix -= 0.001f;
		if (Mix <= 0.f)
			Mix = 0.f;
	}
}

void MouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
	if (FirstMouse)
	{
		LastX = float(xpos);
		LastY = float(ypos);
		FirstMouse = false;
	}

	float xoffset = float(xpos - LastX);
	float yoffset = float(LastY - ypos);//because y is reversed in the screen coordinates
	LastX = float(xpos);
	LastY = float(ypos);

	OurCamera.ProcessMouseMovement(xoffset, yoffset);
}
void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	OurCamera.ProcessMouseScroll(yoffset);
}