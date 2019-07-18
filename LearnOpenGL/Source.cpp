//always make sure to include glad before glfw
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

float Mix;

int main()
{
	
	//make sure that we're using version 3 fir both minor and major
	initGLFW();

	//Generate the Window Object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);//we should make this directly after creating the window
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

	Shader OurShader("VertexShader.glsl", "FragmentShader.glsl");

	/*
	Each vertex attribute takes its data from memory managed by a VBO and which VBO it takes its data from
	(you can have multiple VBOs) is determined by the VBO currently bound to GL_ARRAY_BUFFER
	 when calling glVertexAttribPointer. Since the previously defined VBO is still bound
	before calling glVertexAttribPointer vertex attribute 0 is now associated with its vertex data.
	*/

	//Adding a texture part:

	//Generating a texture:
	unsigned int Texture;
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);


	//set the texture wrapping/filtering options on the current bound texture object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Adding the image:
	//Note image has 0.0 on the top of y-axis while OpenGL expects it to be at the bottom so we have to flip this one 
	stbi_set_flip_vertically_on_load(true);//flip any image you'll load
	int Width, Height, NRChannels;
	unsigned char* Data = stbi_load("container.jpg", &Width, &Height, &NRChannels, 0);
	if (Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
	}
	//free the memory
	stbi_image_free(Data);

	//texture 2
	unsigned int Texture2;
	glGenTextures(1, &Texture2);
	glBindTexture(GL_TEXTURE_2D, Texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//the second image
	unsigned char* Data2 = stbi_load("650833.png", &Width, &Height, &NRChannels, 0);
	if (Data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(Data2);

	float Vertices[] =
	{	//postions			//colors			//texture coords
		0.5f, 0.5f, 0.0f,	1.f, 0.f, 0.f,		1.0f, 1.0f,
	   0.5f, -0.5f, 0.0f,  0.f, 1.f, 0.f,		1.0f, 0.0f,
		-0.5,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};
	unsigned int Indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 *sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	OurShader.Use();//we should activate/use the shader before setting the uniforms
	//both lines do the same to textures but done it differently for each texture for educational purpose
	glUniform1i(glGetUniformLocation(OurShader.ID, "texture1"), 0);
	OurShader.SetInt("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//definition is up there =)

		glfwPollEvents();//checks if any events are triggered like input
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);//we can remove this line because some drivers have it activated by default (0 only)
		glBindTexture(GL_TEXTURE_2D, Texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);

		OurShader.SetFloat("MixValue", Mix);

		OurShader.Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
//if we press the escape key set the 'should close' to true so the window will close
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//if it's not pressed, it returns GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		Mix += 0.001f;
		if (Mix >= 1.f)
			Mix = 1.f;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		Mix -= 0.001f;
		if (Mix <= 0.f)
			Mix = 0.f;
	}
}