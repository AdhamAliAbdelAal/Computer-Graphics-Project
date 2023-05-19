#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>


namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
            // Using the function glCreateProgram() create a shader program and store the ID in the variable program
            program = glCreateProgram();
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program
            // Using the function glDeleteProgram() delete the shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) Return the location of the uniform with the given name
            // Using the function glGetUniformLocation() return the location of the uniform with the given name
            // The first parameter is the program ID
            // The second parameter is the name of the uniform in the shader
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) Send the given float value to the given uniform
            // Using the function glUniform1f() send the given float value to the given uniform
            // The first parameter is the location of the uniform
            // The second parameter is the value to be sent
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
            // the same as the previous one but for unsigned integers
            glUniform1ui(getUniformLocation(uniform), value);
        }

        // ************** This requirement is the same of the previous one **************
        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) Send the given integer value to the given uniform
            // the same as the previous one but for integers
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
            // the same as the previous one but for 2D vectors
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
            // the same as the previous one but for 3D vectors
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
            // the same as the previous one but for 4D vectors
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            // the same as the previous one but for matrices
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
            // the first parameter is the location of the uniform
            // the second parameter is the number of matrices to be sent
            // the third parameter is a boolean that indicates if the matrix should be transposed
            // the fourth parameter is the pointer to the matrix
        }
        void setTime(const std::string &uniform){
            glUniform1f(getUniformLocation(uniform),float(glfwGetTime()));
        }
        // ************** I can't found the copy constructor and assignment operator in the code **************
        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator= (const ShaderProgram&) = delete;
        //Question: Why do we delete the copy constructor and assignment operator?
        /* 
        In my opinion, if the program is copied, the program ID will be the same, and when the destructor is called for one of them, 
        the program ID will be deleted, and the other program will have an invalid ID, so we delete the copy constructor and assignment operator.
        */
        
    };

}

#endif