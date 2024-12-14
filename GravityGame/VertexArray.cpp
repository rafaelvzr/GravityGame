#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "renderer.h"



VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &rendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1,&rendererID));
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    const auto &elements = layout.GetElements();
    intptr_t offset = 0;
    for(unsigned int i = 0;i<elements.size();++i)
    {
        const auto &element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*) offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }   

}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(rendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
