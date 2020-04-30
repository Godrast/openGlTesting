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
		//std::cout << nrChannels;
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		else
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

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

	//this->transparent = false;

}


myTexture::Texture::Texture()
{
	this->index = this->count;
}
myTexture::Texture::Texture(const char* pathToFile)
{
	this->index = this->count;
	//this->transparent = transparent;
	LoadTextureInternal(pathToFile, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_REPEAT, GL_REPEAT);

}



myTexture::Texture::Texture(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2)
{
	this->index = this->count;
	//this->transparent = transparent;
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

//void myTexture::Texture::LoadTextureTransparent(const char* pathToFile, int textureIndex)
//{
//	this->index = this->count;
//
//	if (textureIndex > -1)
//	{
//		this->index = textureIndex;
//	}
//	this->transparent = true;
//
//	LoadTextureInternal(pathToFile, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_REPEAT, GL_REPEAT);
//}

void myTexture::Texture::LoadTexture(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2, int textureIndex)
{
	this->index = this->count;

	if (textureIndex > -1)
	{
		this->index = textureIndex;
	}
	LoadTextureInternal(pathToFile, pname, pname2, param, param2);
}

//void myTexture::Texture::LoadTextureTransparent(const char* pathToFile, GLenum pname, GLenum pname2, GLint param, GLint param2, int textureIndex)
//{
//	this->index = this->count;
//
//	if (textureIndex > -1)
//	{
//		this->index = textureIndex;
//	}
//	this->transparent = true;
//	LoadTextureInternal(pathToFile, pname, pname2, param, param2);
//}

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

void myTexture::Texture::ChangeMap(std::map<const char*, int>& names, int index, const char* newName) const
{

	auto it = names.begin();
	// Iterate through the map
	while (it != names.end())
	{
		// Check if value of this entry matches with given value
		if (it->second == index)
		{
			auto nodeHandler = names.extract(it->first);
			nodeHandler.key() = newName;
			names.insert(std::move(nodeHandler));
			return;
		}
		// Go to next entry in map
		it++;
	}


}

