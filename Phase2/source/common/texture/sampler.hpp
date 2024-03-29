#pragma once

#include <glad/gl.h>
#include <json/json.hpp>
#include <glm/vec4.hpp>

        //Follow This URL for info about GlGenSamplers, These samplers simply override the texture parameters when they're bound       
        //https://stackoverflow.com/questions/28138674/what-is-the-difference-between-glgentextures-and-glgensamplers
namespace our {

    // This class defined an OpenGL sampler
    class Sampler {
        // The OpenGL object name of this sampler 
        GLuint name;
    public:
        // This constructor creates an OpenGL sampler and saves its object name in the member variable "name" 
        Sampler() {
            //TODO: (Req 6) Complete this function
            //This sampler object, when bound, will override the texture sampling parameters.
            //The function generates one sampler and saves its name to the variable name
            glGenSamplers(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL sampler
        ~Sampler() { 
            //TODO: (Req 6) Complete this function
            //Delete the sampler whose value is saved in the variable name
            glDeleteSamplers(1, &name);
         }

        // This method binds this sampler to the given texture unit
        void bind(GLuint textureUnit) const {
            //TODO: (Req 6) Complete this function 

            //Bind a sampler called "name" to a texture unit.
            glBindSampler(textureUnit, name);
        }

        // This static method ensures that no sampler is bound to the given texture unit
        static void unbind(GLuint textureUnit){
            //TODO: (Req 6) Complete this function

            //Unbind a sampler by passing 0 as the sampler name
            glBindSampler(textureUnit, 0);
        }

        // This function sets a sampler paramter where the value is of type "GLint"
        // This can be used to set the filtering and wrapping parameters
        void set(GLenum parameter, GLint value) const {
            //TODO: (Req 6) Complete this function

            //Given a parameter and a GLint value to be assigned to it, pass both alongside the Sampler "name" to the function
            //glSamplerParameteri
            glSamplerParameteri(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat"
        // This can be used to set the "GL_TEXTURE_MAX_ANISOTROPY_EXT" parameter
        void set(GLenum parameter, GLfloat value) const {
            //TODO: (Req 6) Complete this function
            //Given a parameter and a GLfloat value to be assigned to it, pass both alongside the Sampler "name" to the function
            //glSamplerParameterf
            glSamplerParameterf(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat[4]"
        // This can be used to set the "GL_TEXTURE_BORDER_COLOR" parameter
        void set(GLenum parameter, glm::vec4 value) const {
            glSamplerParameterfv(name, parameter, &(value.r));
        }

        // Given a json object, this function deserializes the sampler state
        void deserialize(const nlohmann::json& data);

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };

}