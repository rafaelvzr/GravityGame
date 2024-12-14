#include"Shader.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include "renderer.h"
#include <codecvt> 

Shader::Shader(const std::string &filepath)
    :filepath(filepath)
{
    ShaderProgramSource source = ParseShader(filepath);
	rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(rendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(rendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}


ShaderProgramSource Shader::ParseShader(const std::string &filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while(getline(stream, line)){

		if(line.find("#shader") != std::string::npos){
			if(line.find("vertex") != std::string::npos){
				type = ShaderType::VERTEX;
			}
			else if(line.find("fragment") != std::string::npos){
				type = ShaderType::FRAGMENT;
			}
		}
		else{
			ss[(int)type] << line << '\n';
		}
	}	
	return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE){
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;	
		std::cout<< message << std::endl;
		glDeleteShader(id);
		delete[] message;
		message = nullptr;
		return 0;
	}

	return id;
}


unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

void Shader::setUniform4f(const std::string &name, float f0, float f1, float f2, float f3)
{
    GLCall(glUniform4f(GetUniformLocation(name),f0,f1,f2,f3));
}
void Shader::setUniform3f(const std::string &name, float f0, float f1, float f2)
{
    GLCall(glUniform3f(GetUniformLocation(name),f0,f1,f2));
}

void Shader::setUniform2f(const std::string &name, float f0, float f1)
{
	GLCall(glUniform2f(GetUniformLocation(name),f0,f1));
}

void Shader::setUniform1i(const std::string &name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name),value));
}

void Shader::setUniform1iv(const std::string &name, int count, int *samplers)
{
	GLCall(glUniform1iv(GetUniformLocation(name),count,samplers));
}

void Shader::setUniform1f(const std::string &name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name),value));
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name),1,GL_FALSE,&matrix[0][0]));
}

unsigned int Shader::GetUniformLocation(const std::string &name)
{
	auto x = uniformLocationCache.find(name);
	if (x != uniformLocationCache.end()) return x->second;
	GLCall(int location =  glGetUniformLocation(rendererID, name.c_str()));
	if(location != -1) std::cout << "Warning: Uniform " << name << " does not exist." << std::endl;
	uniformLocationCache[name] = location;
	return location;
}


