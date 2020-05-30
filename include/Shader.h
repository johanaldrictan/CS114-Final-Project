#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// the program ID
	unsigned int ID;
	// constructor
	Shader() { }
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// sets the current shader as active
	Shader& use();
	// compiles the shader from given source code
	void compile(const char* vertexSource, const char* fragmentSource);
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const char* name, float value, bool useShader = false);
	void setInteger(const char* name, int value, bool useShader = false);
	void setVector2f(const char* name, float x, float y, bool useShader = false);
	void setVector2f(const char* name, const glm::vec2& value, bool useShader = false);
	void setVector3f(const char* name, float x, float y, float z, bool useShader = false);
	void setVector3f(const char* name, const glm::vec3& value, bool useShader = false);
	void setVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
	void setVector4f(const char* name, const glm::vec4& value, bool useShader = false);
	void setMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type);
};
#endif