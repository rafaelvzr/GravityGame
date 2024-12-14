#pragma once


class IndexBuffer{
private:
    unsigned int rendererID;
    unsigned int count;
public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;
    bool AppendData(unsigned newCount, const unsigned* newData);

    inline unsigned int getCount() const {return count;}
};