#include<iostream>
#include"renderer.h"

void GLClearError(){
	while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line){
	while(GLenum error = glGetError()){
		std::cout << "[OPEN GL ERROR] (" << error << "): " << function << " " << file << ":" << line << std::endl; 
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setClearColor(float r, float g, float b, float a) const
{
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawTriangles(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));	
}

void Renderer::DrawTriangles(const VertexArray &va,GLint first, GLuint count, const Shader &shader) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, first, count));
}

void Renderer::DrawPoints(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_POINTS, ib.getCount(), GL_UNSIGNED_INT, nullptr));	
}

void Renderer::DrawPoints(const VertexArray &va,GLint first, GLuint count, const Shader &shader) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_POINTS, first, count));
}

void Renderer::DrawLines(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_LINES, ib.getCount(), GL_UNSIGNED_INT, nullptr));	
}

void Renderer::DrawLines(const VertexArray &va,GLint first, GLuint count, const Shader &shader) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_LINES, first, count));
}
