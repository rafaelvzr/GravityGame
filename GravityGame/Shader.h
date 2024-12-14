#pragma once
#include<string>
#include<unordered_map>
#include"vendor/glm/glm.hpp"

struct ShaderProgramSource{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader{
private:
    std::string filepath;
    unsigned int rendererID;
    std::unordered_map<std::string,int> uniformLocationCache;
public:
    Shader(const std::string &filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    inline unsigned int getRendererID() const {return rendererID;}

    void setUniform4f(const std::string &name, float f0, float f1, float f2, float f3);
    void setUniform3f(const std::string & name, float f0, float f1, float f2);
    void setUniform2f(const std::string & name, float f0, float f1);
    void setUniform1i(const std::string &name, int value);
    void setUniform1iv(const std::string &name, int count ,int *samplers);
    void setUniform1f(const std::string &name, float value);
    void setUniformMat4f(const std::string &name, const glm::mat4 &matrix);
    static ShaderProgramSource ParseShader(const std::string &filepath);
    static unsigned int CompileShader(unsigned int type, const std::string &source);   
    static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
private:
    unsigned int GetUniformLocation(const std::string &name);
};