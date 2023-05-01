#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const
{
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *sourceCStr = sourceString.c_str();
    file.close();

    // TODO: Complete this function
    // Note: The function "checkForShaderCompilationErrors" checks if there is
    //  an error in the given shader. You should use it to check if there is a
    //  compilation error and print it so that you can know what is wrong with
    //  the shader. The returned string will be empty if there is no errors.

    // We return true if the compilation succeeded

    // create the shader object of the given type
    GLuint shader = glCreateShader(type);

    // attach the shader source code to the shader object
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    // you may give the glShaderSource function an array of strings to combine them into one shader source
    // the last parameter is an array of integers that specify the length of each string in the array
    // (or NULL to indicate that each string is null-terminated)


    // compile the shader
    glCompileShader(shader);
    // check for compilation errors
    // if there is an error in the compilation process, print the error message and return false
    string s=checkForShaderCompilationErrors(shader);
    if (s != "")
    {
        cout <<"error in shader\n";
        cout << s << endl;
        return false;
    }
    // attach the shader to the program
    glAttachShader(program, shader);
    
    //Delete the shader object after attaching it to the program
    glDeleteShader(shader);

    //Return true if the compilation succeeded
    return true;
}

bool our::ShaderProgram::link() const
{
    // TODO: Complete this function
    // Note: The function "checkForLinkingErrors" checks if there is
    //  an error in the given program. You should use it to check if there is a
    //  linking error and print it so that you can know what is wrong with the
    //  program. The returned string will be empty if there is no errors.
    glLinkProgram(program);
    string s=checkForLinkingErrors(program);
    // if there is an error in the linking process, print the error message and return false
    if (s != "")
    {
        //Print the Error Message in case of failure
        cout << s << endl;
        return false;
    }

    // We return true if the linking succeeded
    return true;
    
}
    ////////////////////////////////////////////////////////////////////
    // Function to check for compilation and linking error in shaders //
    ////////////////////////////////////////////////////////////////////

    std::string checkForShaderCompilationErrors(GLuint shader)
    {
        // Check and return any error in the compilation process
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            GLint length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            char *logStr = new char[length];
            glGetShaderInfoLog(shader, length, nullptr, logStr);
            std::string errorLog(logStr);
            delete[] logStr;
            return errorLog;
        }
        return std::string();
    }

    std::string checkForLinkingErrors(GLuint program)
    {
        // Check and return any error in the linking process
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (!status)
        {
            GLint length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            char *logStr = new char[length];
            glGetProgramInfoLog(program, length, nullptr, logStr);
            std::string error(logStr);
            delete[] logStr;
            return error;
        }
        return std::string();
    }