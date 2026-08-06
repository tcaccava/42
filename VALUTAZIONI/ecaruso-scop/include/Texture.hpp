#pragma once

#include <string>

class Texture
{
private:
    unsigned int _ID;
    int _width;
    int _height;
	bool _valid;

    bool loadPPM(const std::string& path);

public:
    Texture(const std::string& path);
    ~Texture();

    void bind() const;
	bool isValid() const;
};