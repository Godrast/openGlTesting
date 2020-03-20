#ifndef  LoadTexture_HPP
#define LoadTexture_HPP
#include "stb_image.h"
#include <GL/glew.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <stdarg.h>
#include <map>
#include <iterator>

namespace myTexture
{
	class Texture
	{

	private:
		std::vector<GLuint> textureIDs;
		int index = 0;
		//bool transparent = false;
		bool flip = false;
		int count = 0;

		void LoadTextureInternal(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2);
		//void LoadTextureInternalTransparent(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2);

	public:
		Texture();
		Texture(const char* pathToFile);

		Texture(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2);

		void FlipNextImageVertically();
		void LoadTexture(const char* pathToFile, int textureIndex = -1);
		//useless
		//void LoadTextureTransparent(const char* pathToFile, int textureIndex = -1);
		void LoadTexture(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2, int textureIndex = -1);
		//useless
		//void LoadTextureTransparent(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2, int textureIndex = -1);
		void SetActive(int index, ...); // most likely useless
		void ChangeMap(std::map<const char*, int>& names,  int index, const char* newName) const;
	};

}



#endif // ! LoadTexture_HPP
