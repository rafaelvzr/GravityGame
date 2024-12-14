#include "VertexBuffer.h"
#include "renderer.h"



VertexBuffer::VertexBuffer(bool isStatic, const void *data, unsigned int size)
	: isStatic(isStatic)
{
	if (isStatic) {
		GLCall(glGenBuffers(1, &rendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}
	else {
		GLCall(glGenBuffers(1, &rendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	}
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1,&rendererID));
}

//increase/decrease size of VBO
//eg:create/destroy elements to add/remove them from the scene 
bool VertexBuffer::AppendData(unsigned newSize, const float *newVertices) const
{
	if (isStatic) {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, newSize, newVertices, GL_STATIC_DRAW));
		return true;
	}
	else {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, newSize, newVertices, GL_DYNAMIC_DRAW));
		return true;
	}
	return false;
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

//update dynamicly the VBO, every onUpdate() function
//eg: changing position without creating and destroying VBOs
bool VertexBuffer::UpdateData(float *vertices,unsigned size) const
{
	if (isStatic) return false;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER,0,size,vertices));
	return true;
}

