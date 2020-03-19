#include "LoadTexture.hpp"


void myTexture::Texture::LoadTextureInternal(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2)
{



	if (this->index <= textureIDs.size())
	{
		textureIDs.push_back((GLuint)0);
	}

	glGenTextures(1, &(this->textureIDs[this->index]));
	glActiveTexture(GL_TEXTURE0 + this->index);
	glBindTexture(GL_TEXTURE_2D, this->textureIDs[this->index]);


	glTexParameteri(GL_TEXTURE_2D, pname, param);
	glTexParameteri(GL_TEXTURE_2D, pname2, param2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);




	stbi_set_flip_vertically_on_load(this->flip);
	this->flip = false;


	int width, height, nrChannels;
	unsigned char* data = stbi_load(pathToFile, &width, &height, &nrChannels, 0);

	if (data)
	{

		if (this->transparent)
		{

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		if (this->index == this->count)
		{
			this->count++;
		}

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		std::cout << stbi_failure_reason();

	}



	stbi_image_free(data);

	this->transparent = false;

}


myTexture::Texture::Texture()
{
	this->index = this->count;
}
myTexture::Texture::Texture(const char* pathToFile, bool transparent)
{
	this->index = this->count;
	this->transparent = transparent;
	LoadTextureInternal(pathToFile, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_REPEAT, GL_REPEAT);

}



myTexture::Texture::Texture(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2, bool transparent)
{
	this->index = this->count;
	this->transparent = transparent;
	LoadTextureInternal(pathToFile, pname, pname2, param, param2);

}

void myTexture::Texture::FlipNextImageVertically()
{
	this->flip = true;
}

void myTexture::Texture::LoadTexture(const char* pathToFile, int textureIndex)
{
	this->index = this->count;

	if (textureIndex > -1)
	{
		this->index = textureIndex;
	}

	LoadTextureInternal(pathToFile, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_REPEAT, GL_REPEAT);
}

void myTexture::Texture::LoadTextureTransparent(const char* pathToFile, int textureIndex)
{
	this->index = this->count;

	if (textureIndex > -1)
	{
		this->index = textureIndex;
	}
	this->transparent = true;

	LoadTextureInternal(pathToFile, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_REPEAT, GL_REPEAT);
}

void myTexture::Texture::LoadTexture(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2, int textureIndex)
{
	this->index = this->count;

	if (textureIndex > -1)
	{
		this->index = textureIndex;
	}
	LoadTextureInternal(pathToFile, pname, pname2, param, param2);
}

void myTexture::Texture::LoadTextureTransparent(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2, int textureIndex)
{
	this->index = this->count;

	if (textureIndex > -1)
	{
		this->index = textureIndex;
	}
	this->transparent = true;
	LoadTextureInternal(pathToFile, pname, pname2, param, param2);
}

void myTexture::Texture::SetActive(int index, ...)
{
	va_list args;
	va_start(args, index);

	while (index != '\0')
	{
		glActiveTexture(GL_TEXTURE0 + index);
	}

	va_end(args);
}

