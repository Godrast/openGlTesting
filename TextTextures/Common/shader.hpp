#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <map>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Light.cpp"

class Shader
{

private:
	GLuint ProgramID;
	std::map<const char*, own::Light *> lightSources;
	void checkCompileErrors(GLuint shader, std::string type);
	void setLight(own::Light *currentLight, int index);

public:
	void LoadShadersExternal(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void LoadShaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

	void addLight(const char* name, own::Light *light);
	void useLights();
	//void useLights(const char * names);
	void use();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, GLsizei size, GLfloat* value, GLboolean transpose = GL_FALSE) const;
	void setMat4(const std::string& name, const glm::mat4 &mat) const;
	//void setVec3(const std::string& name, glm::vec3 value) const;


	void setVec2(const std::string& name, const glm::vec2 &value) const;
	void setVec2(const std::string& name, float x, float y) const;

	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;

	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;



};


#endif