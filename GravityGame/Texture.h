#pragma once
#include <string>
#include"renderer.h"

class Texture
{
private:
    unsigned int rendererID;
    std::string filepath;
    unsigned char *localBuffer;
    int width, height, BPP;
public:
    Texture(const std::string &filepath);
    ~Texture();

    void setNewTexture(const std::string &filepath);
    void Bind(unsigned int slot = 0) const;
    void UnBind() const;

    inline int getWidth() const {return width;}
    inline int getHeight() const {return height;}
    inline const unsigned char *getBuffer() const { return localBuffer; }
    inline unsigned int getRendererID() const { return rendererID; }
};