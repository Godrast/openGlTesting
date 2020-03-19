#ifndef FRAGMENT_SHADERS_HPP
#define FRAGMENT_SHADERS_HPP

class FragmentShaders {
	const char * shader;
	const char* tempShader;

public:
	FragmentShaders();
	const char* getShader();
	const char * getTempShader();
};
#endif // !FRAGMENT_SHADERS_HPP
