#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>


class Shader
{
public:
	unsigned int ID;

	Shader(const GLchar* VertexPath, const GLchar* FragmentPath);

	void Use();
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string& name, float value) const;

	//default constructor and destructors
	~Shader();

private:
	void CheckCompileErrors(unsigned int Shader, std::string type);
};

#endif