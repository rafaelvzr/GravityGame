#pragma once
#include"vector"
#include<include/glad/glad.h>
#include "renderer.h"
#include<stdexcept>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type){
        switch (type)
        {
        case GL_FLOAT:          return 4;
        case GL_UNSIGNED_INT:   return 4;
        case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout{
private:
    std::vector<VertexBufferElement> Elements;
    unsigned int stride;
public:
    VertexBufferLayout()
        : stride(0) 
        {}

    template<typename T>
    void Push(unsigned int count);


    inline const std::vector<VertexBufferElement> &GetElements() const {return Elements;}
    inline unsigned int GetStride() const {return stride;}
};

template<> inline   
void VertexBufferLayout::Push<float>(unsigned int count){
    Elements.push_back({GL_FLOAT,count,GL_FALSE});
    stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template<> inline
void VertexBufferLayout::Push<unsigned int>(unsigned int count){
    Elements.push_back({GL_UNSIGNED_INT,count,GL_FALSE});
    stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<> inline
void VertexBufferLayout::Push<unsigned char>(unsigned int count){
    Elements.push_back({GL_UNSIGNED_BYTE,count,GL_TRUE});
    stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}