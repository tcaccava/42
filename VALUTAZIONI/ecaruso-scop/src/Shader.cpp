#include "Shader.hpp"

    std::string Shader::readFile(const std::string& path){
        
        std::stringstream buffer;
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return "";
        }
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
	}

    void Shader::compileShader(const std::string& source, unsigned int shaderType, unsigned int& shaderID){
        const char* shaderCode = source.c_str();
        shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &shaderCode, nullptr);
        glCompileShader(shaderID);
        std::string type = (shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        checkErrors(shaderID, type);
    }

    unsigned int Shader::createProgram(unsigned int vertexShader, unsigned int fragmentShader){
        unsigned int programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);
        checkErrors(programID, "PROGRAM");
        return programID;
    }

    void Shader::checkErrors(unsigned int shader, const std::string& type){
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath){
        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);

        unsigned int vertexShader, fragmentShader;
        compileShader(vertexCode, GL_VERTEX_SHADER, vertexShader);
        compileShader(fragmentCode, GL_FRAGMENT_SHADER, fragmentShader);

        _ID = createProgram(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader(){
        glDeleteProgram(_ID);
    }

    void Shader::use() const{
        glUseProgram(_ID);
    }

	void Shader::setMatrix4(const std::string& name, const Matrix4& matrix) const{
		unsigned int location = glGetUniformLocation(_ID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data());
	}

	void Shader::setBool(const std::string& name, bool value) const{
		unsigned int location = glGetUniformLocation(_ID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::setInt(const std::string& name, int value) const
{
    unsigned int location = glGetUniformLocation(_ID, name.c_str());
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value)
{
    int location = glGetUniformLocation(_ID, name.c_str());
    glUniform1f(location, value);
}