#include "fragmentShaders.hpp"

FragmentShaders::FragmentShaders() {
	shader = "#version 330 core\n"

		//"in vec3 fragmentColor;\n"
		"in vec3 myColor;\n"
		"in vec2 texCoord;\n"

		"// Output data\n"
		"out vec4 color;\n"
		"uniform sampler2D texture1;\n"
		"uniform sampler2D texture2;\n"
		"uniform float interpolation;\n"
		"void main() {\n"
			//"color = vec4(myColor, 1.0);\n"
			"color = texture(texture2, texCoord);// * vec4(myColor, 1.0);\n"
			"vec4 temp = texture(texture2, vec2(1 - texCoord.x, texCoord.y));\n"
			//"temp = texture(texture2, texCoord);\n"
			//"color = mix(texture(texture1, texCoord), temp, interpolation);// * vec4(myColor, 1.0);\n"
		"}";

	tempShader = "#version 330 core\n"

		//"in vec3 fragmentColor;\n"
		"in vec3 myColor;\n"
		//"in vec2 texCoord;\n"

		"// Output data\n"
		"out vec4 color;\n"
		//"uniform sampler2D texture1;\n"
		//"uniform sampler2D texture2;\n"
		"uniform float alpha;\n"
		"void main() {\n"
		"	color = vec4(myColor, alpha);\n"
		//"color = texture(texture2, texCoord) * vec4(myColor, 1.0);\n"
		//"vec4 temp = texture(texture2, vec2(1 - texCoord.x, texCoord.y));\n"
		//"temp = texture(texture2, texCoord);\n"
		//"color = mix(texture(texture1, texCoord), temp, interpolation) * vec4(myColor, 1.0);\n"
		"}";

}

const char * FragmentShaders::getShader() {
	return shader;
}

const char* FragmentShaders::getTempShader()
{
	return tempShader;
}

