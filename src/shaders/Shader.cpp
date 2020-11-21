#include <string>
#include <GL\glew.h>
#include <iostream>
#include <sstream>
#include <fstream>


struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

/// <summary>
/// Reads shader code from file in filepath.
/// </summary>
/// <param name="filepath">of the shader.</param>
/// <returns>The shader strings.</returns>
static ShaderProgramSource ParseShader(const std::string& filepath)
{
    /* Load the shader file using a filestream */
    std::ifstream stream(filepath);

    /* Create the shader type enum */
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    /* Define some variables */
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    /* Loop over every line in the shader file */
    while (getline(stream, line))
    {
        /* If there is a shader tag */
        if (line.find("#shader") != std::string::npos)
        {
            /* Check for each shader type */
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else if (type != ShaderType::NONE) {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

/// <summary>
/// Compiles the given source code into a shader.
/// </summary>
/// <param name="type">of the shader.</param>
/// <param name="source">code.</param>
/// <returns>The shader ID</returns>
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    /* Create the shader */
    unsigned int id = glCreateShader(type);

    /* Load the source code into the shader */
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);

    /* Compile the shader */
    glCompileShader(id);

    /* Load the result */
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    /* Check if the result is an error */
    if (result == GL_FALSE)
    {
        /* Get the length of the error */
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        /* Get the message */
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        /* Write error to the console */
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        /* Delete the shader */
        glDeleteShader(id);

        return 0;
    }

    return id;
}

/// <summary>
/// Create a vertex and fragment shader.
/// </summary>
/// <param name="vertexShader">source code.</param>
/// <param name="fragmentShader">source code.</param>
/// <returns>Program ID</returns>
static unsigned int CreateShaderVF(const std::string& vertexShader, const std::string& fragmentShader)
{
    /* Create the program */
    unsigned int program = glCreateProgram();

    /* Compile the shaders */
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* Attack the shaders to the program */
    //glAttachShader(program, vs);
    glAttachShader(program, fs);

    /* Link and validate the program */
    glLinkProgram(program);
    glValidateProgram(program);

    /* Delete the shaders */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}