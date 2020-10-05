#include "Shader.h"

Shader::Shader(const std::filesystem::path &vertexShaderCode, const std::filesystem::path &fragmentShaderCode) {
    std::string VertexCodeStr;
    std::string FragmentCodeStr;

    std::fstream VertexCodeFile;
    std::fstream FragmentCodeFile;

    VertexCodeFile.exceptions(std::fstream::failbit | std::fstream::badbit);
    FragmentCodeFile.exceptions(std::fstream::failbit | std::fstream::badbit);

    try {
        std::stringstream VertexStream;
        std::stringstream FragmentStream;

        VertexCodeFile.open(vertexShaderCode);
        FragmentCodeFile.open(fragmentShaderCode);

        VertexStream << VertexCodeFile.rdbuf();
        FragmentStream << FragmentCodeFile.rdbuf();

        VertexCodeFile.close();
        FragmentCodeFile.close();

        VertexCodeStr = VertexStream.str();
        FragmentCodeStr = FragmentStream.str();

    } catch (std::fstream::failure & except) {
        std::cerr << vertexShaderCode << '\n' << fragmentShaderCode << std::endl;
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << except.what() << std::endl;
    }

    const char * c_VertexCode = VertexCodeStr.c_str();
    const char * c_FragmentCode = FragmentCodeStr.c_str();

    unsigned int VertexId, FragmentId;

    this->ID = glCreateProgram();

    VertexId = glCreateShader(GL_VERTEX_SHADER);
    FragmentId = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(VertexId, 1, &c_VertexCode, nullptr);
    glShaderSource(FragmentId, 1, &c_FragmentCode, nullptr);

    glCompileShader(VertexId);
    glCompileShader(FragmentId);

    Check(VertexId);
    Check(FragmentId);

    glAttachShader(this->ID, VertexId);
    glAttachShader(this->ID, FragmentId);

    glLinkProgram(this->ID);

    int  success;
    char infoLog[512];
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(VertexId);
    glDeleteShader(FragmentId);
}

bool Shader::Check(unsigned int & Shader) {
    int  success;      // проверка на успешную компиляцию шейдера
    char infoLog[512];
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(Shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    return true;
}


