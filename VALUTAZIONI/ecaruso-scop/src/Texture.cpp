#include "Texture.hpp"

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

Texture::Texture(const std::string& path)
    : _ID(0), _width(0), _height(0), _valid(false)
{
	if (!loadPPM(path))
	{
		std::cerr << "Failed to load texture: " << path << std::endl;
		return;
	}
	_valid = true;
}

Texture::~Texture()
{
    if (_ID != 0)
        glDeleteTextures(1, &_ID);
}

void Texture::bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _ID);
}

bool Texture::loadPPM(const std::string& path)
{
    std::ifstream file(path.c_str(), std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Failed to open texture: "
                  << path << std::endl;
        return false;
    }

    std::string format;
    file >> format;

    if (format != "P6")
    {
        std::cerr << "Unsupported PPM format: "
                  << format << std::endl;
        return false;
    }

    file >> _width >> _height;

    int maxValue;
    file >> maxValue;

    if (_width <= 0 || _height <= 0 || maxValue != 255)
    {
        std::cerr << "Invalid PPM texture" << std::endl;
        return false;
    }

    file.get();

    std::vector<unsigned char> pixels(
        _width * _height * 3
    );

    file.read(
        reinterpret_cast<char*>(pixels.data()),
        pixels.size()
    );

    if (!file)
    {
        std::cerr << "Failed to read texture data"
                  << std::endl;
        return false;
    }

    glGenTextures(1, &_ID);
    glBindTexture(GL_TEXTURE_2D, _ID);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        _width,
        _height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );

    glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool Texture::isValid() const
{
	return _valid;
}
