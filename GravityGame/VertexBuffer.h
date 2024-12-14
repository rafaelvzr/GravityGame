#pragma once

#define VERTEX_BUFFER_STATIC	true
#define VERTEX_BUFFER_DYNAMIC	false

class VertexBuffer{
private:
    unsigned int rendererID;
    bool isStatic;
public:
    VertexBuffer(bool isStatic,  const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
    bool UpdateData(float vertices[], unsigned size) const;
    bool AppendData(unsigned newSize, const float* newVertices) const;
};