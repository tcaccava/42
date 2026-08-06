#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Math/Matrix4.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

class Shader {
    private:
        unsigned int _ID;

        std::string readFile(const std::string& path);
        void compileShader(const std::string& source, unsigned int shaderType, unsigned int& shaderID);
        unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
        void checkErrors(unsigned int shader, const std::string& type);

    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        void use() const;

		void setMatrix4(const std::string& name, const Matrix4& matrix) const;

		void setBool(const std::string& name, bool value) const;

		void setInt(const std::string& name, int value) const;

		void setFloat(const std::string& name, float value);
};