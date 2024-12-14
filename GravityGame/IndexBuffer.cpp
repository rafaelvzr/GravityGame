#include "IndexBuffer.h"
#include "renderer.h"


IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) :
    count(count)
{
    GLCall(glGenBuffers(1, &rendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1,&rendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//increase/decrease size of VBO
//eg:create/destroy elements to add/remove them from the scene 
bool IndexBuffer::AppendData(unsigned newCount, const unsigned* newData)
{
	count = newCount;
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, newCount * sizeof(unsigned int), newData, GL_STATIC_DRAW));
	return false;
}
