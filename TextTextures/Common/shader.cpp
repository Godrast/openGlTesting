
#include "shader.hpp"


void  Shader::LoadShadersExternal(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}


	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}

	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertex);
	glAttachShader(ProgramID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(ProgramID, geometry);
	glLinkProgram(ProgramID);
	checkCompileErrors(ProgramID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);

}


void Shader::LoadShaders(const char* vertexShader, const char* fragmentShader, const char* geometryShader)
{


	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShader, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShader, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (geometryShader != nullptr)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometryShader, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertex);
	glAttachShader(ProgramID, fragment);
	if (geometryShader != nullptr)
		glAttachShader(ProgramID, geometry);
	glLinkProgram(ProgramID);
	checkCompileErrors(ProgramID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryShader != nullptr)
		glDeleteShader(geometry);



}


// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}



void Shader::addLight(const char* name, own::Light* light)
{
	lightSources.insert({ name, light });
}

void Shader::setLight(own::Light* currentLight, int index)
{

	std::string lightNum = "lights[" + std::to_string(index) + "].";
	//std::cout << "name: " << lightNum << "\n";
	//std::cout << glm::to_string(currentLight->direction);

	setInt(lightNum + "type", currentLight->type);
	setVec3(lightNum + "diffuse", currentLight->diffuse);
	setVec3(lightNum + "specular", currentLight->specular);
	setVec3(lightNum + "ambient", currentLight->ambient);

	switch (currentLight->type)
	{

		case own::SPOTLIGHT:

			setFloat(lightNum + "cutOff", currentLight->cutOff);
			setFloat(lightNum + "outerCutOff", currentLight->outerCutOff);
			setVec3(lightNum + "direction", currentLight->direction);
			//there is no break for a reason
			//don't add it
			//it's there to avoid at least some repeated code
		case own::POINT:

			setFloat(lightNum + "constant", currentLight->constant);
			setFloat(lightNum + "linear", currentLight->linear);
			setFloat(lightNum + "quadratic", currentLight->quadratic);

			setVec3(lightNum + "position", currentLight->position);
			break;
		case own::DIRECTIONAL:

			setVec3(lightNum + "direction", currentLight->direction);
			break;

	}

}

void Shader::useLights()
{
	int counter = 0;
	auto light = lightSources.begin();
	while (light != lightSources.end())
	{
		setLight(light->second, counter);
		light++;
		counter++; //i know it could be in the argument, but this is cleaner
	}

	setInt("numOfLights", counter);

}

void Shader::use()
{
	glUseProgram(ProgramID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, GLsizei size, GLfloat* value, GLboolean transpose) const
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), size, transpose, value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3f(glGetUniformLocation(ProgramID, name.c_str()), value[0], value[1], value[2]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4f(glGetUniformLocation(ProgramID, name.c_str()), value[0], value[1], value[2], value[3]);
}



void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ProgramID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
//void Shader::Shader::setVec3(const std::string& name, const glm::vec3& value) const
//{
//	glUniform3fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);
//}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ProgramID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
//void Shader::setVec4(const std::string& name, const glm::vec4& value) const
//{
//	glUniform4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);
//}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ProgramID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
//void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
//{
//	glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//}
//

