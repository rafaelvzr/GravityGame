#pragma once
#include<include/glad/glad.h>
#include "VertexArray.h"
#include"IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();x;ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

class Renderer
{
private:
public:
    void Clear() const;
    void setClearColor(float r, float g, float b, float a) const;

    void DrawTriangles(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawTriangles(const VertexArray& va,GLint first, GLuint count,const Shader& shader) const;

    void DrawPoints(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawPoints(const VertexArray& va,GLint first, GLuint count,const Shader& shader) const;

    void DrawLines(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawLines(const VertexArray& va,GLint first, GLuint count,const Shader& shader) const;
};