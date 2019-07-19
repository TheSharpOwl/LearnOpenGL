#include "Shader.h"

Shader::Shader(const GLchar* VertexPath, const GLchar* FragmentPath)
{
	std::string VertexCode, FragmentCode;
	std::ifstream VShaderFile, FShaderFile;

	VShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	FShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		VShaderFile.open(VertexPath);
		FShaderFile.open(FragmentPath);
		std::stringstream VShaderStream, FShaderStream;

		VShaderStream << VShaderFile.rdbuf();
		FShaderStream << FShaderFile.rdbuf();

		VShaderFile.close();
		FShaderFile.close();

		VertexCode = VShaderStream.str();
		FragmentCode = FShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = VertexCode.c_str();
	const char* fShaderCode = FragmentCode.c_str();

	//compile shaders 
	unsigned int FragmentShader, VertexShader;

	//vertex shader
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vShaderCode, NULL);
	glCompileShader(VertexShader);
	CheckCompileErrors(VertexShader, "Vertex");

	//fragment shader
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(FragmentShader);
	CheckCompileErrors(FragmentShader, "Fragment");

	ID = glCreateProgram();
	glAttachShader(ID, VertexShader);
	glAttachShader(ID, FragmentShader);
	glLinkProgram(ID);
	CheckCompileErrors(ID, "Program");

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

Shader::~Shader()
{
}

void Shader::CheckCompileErrors(unsigned int ToTest, std::string type)
{
	int Success;
	char infoLog[1024];
	
	if (type != "Program")// I made it lower case
	{
		glGetShaderiv(ToTest, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(ToTest, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
				<< "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(ToTest, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog(ToTest, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
				<< "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
void Shader::SetVec2(const std::string& name, const glm::vec2 value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()),1, &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4 value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}



void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
	//3rd argument if we want to transpose it, 4th the data but we transform it so it'll be appropirate for OpenGL
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	//3rd argument if we want to transpose it, 4th the data but we transform it so it'll be appropirate for OpenGL
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	//3rd argument if we want to transpose it, 4th the data but we transform it so it'll be appropirate for OpenGL
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}