//always make sure to include glad before glfw
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

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

//if we press the escape key set the 'should close' to true so the window will close
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//if it's not pressed, it returns GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
}

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

static std::string ParseShader(const std::string& FilePath)
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

static void CheckShader(const unsigned int &Shader)
{
	GLint Success;
	char Log[513];
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{
		glGetShaderInfoLog(Shader, 513, NULL, Log);
		std::cout << "Shader Error Comilation Failed\n" << Log << std::endl;
	}
}

static void CheckProgram(const unsigned int& Program)
{
	GLint Success;
	char Log[513];
	glGetProgramiv(Program, GL_LINK_STATUS, &Success);
	
	if (!Success)
	{
		glGetProgramInfoLog(Program, 513, NULL, Log);
		std::cout << "Error in the shader program \n" << Log << std::endl;
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

	
	//Vertex Shader Part
	unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string GetVertexShaderCode = ParseShader("VertexShader.shader");
	const char* VertexShaderCode = GetVertexShaderCode.c_str();
	glShaderSource(VertexShader, 1, &VertexShaderCode, NULL);
	glCompileShader(VertexShader);
	CheckShader(VertexShader);

	//Fragment Shader Part
	unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string GetFragmentShaderCode = ParseShader("FragmentShader.shader");
	const char* FragmentShaderCode = GetFragmentShaderCode.c_str();
	glShaderSource(FragmentShader, 1, &FragmentShaderCode, NULL);
	glCompileShader(FragmentShader);
	CheckShader(FragmentShader);

	//Shader program part
	unsigned int ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);
	CheckProgram(ShaderProgram);

	//we don't need the shaders anymore
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	/*
	Each vertex attribute takes its data from memory managed by a VBO and which VBO it takes its data from
	(you can have multiple VBOs) is determined by the VBO currently bound to GL_ARRAY_BUFFER
	 when calling glVertexAttribPointer. Since the previously defined VBO is still bound
	before calling glVertexAttribPointer vertex attribute 0 is now associated with its vertex data.
	*/
	unsigned int VAO, VBO, EBO;

	float Vertices[] =
	{	//postions			//colors
		0.5f, -0.5f, 0.0f,	 1.f, 0.f, 0.f, 
	   -0.5f, -0.5f, 0.0f,  0.f, 1.f, 0.f,
		0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	unsigned int Indices[] =
	{
		0, 1, 2
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 *sizeof(float)));
	glEnableVertexAttribArray(1);


	//glUseProgram(ShaderProgram);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//definition is up there =)

		glfwPollEvents();//checks if any events are triggered like input
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(ShaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}