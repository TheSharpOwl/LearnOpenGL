//always make sure to include glad before glfw
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

void initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//if we press the escape key set the 'should close' to true so the window will close
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//if it's not pressed, it returns GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
}

//Function for reading a shader file given the path
std::string ParseShader(const std::string& FilePath)
{
	std::ifstream Stream(FilePath);
	std::string Line;
	std::stringstream Code;
	while (getline(Stream, Line))
	{
		Code << Line << "\n";
	}

	return Code.str();
}

void Check(unsigned int &Shader)
{
	GLint success;
	char log[513];
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(Shader, 513, NULL, log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
	}
}
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

	float Vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	//define a vertex buffer object
	unsigned int VertexBufferObjectID;
	glGenBuffers(1, &VertexBufferObjectID);
	//bind the Buffer to an buffer array
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjectID);
	// From that point on any buffer calls we make will be used to configure the currently bound buffer......

	//copy the data from 'Vertices[]' to the current buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	//shaders part
	unsigned int VertexShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string VertexShaderCode = ParseShader("VertexShader.shader");
	const char* temp = VertexShaderCode.c_str();
	glShaderSource(VertexShader, 1, &temp, NULL);
	glCompileShader(VertexShader);
	Check(VertexShader);
	//simple 'render loop' which keeps our window open until the user closes it and we put the rendering commands inside it.
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//definition is up there =)

		glfwSwapBuffers(window);
		glfwPollEvents();//checks if any events are triggered like input
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

	}

	//properly clean/delete all of GLFW's resources that were allocated
	glfwTerminate();

	return 0;
}