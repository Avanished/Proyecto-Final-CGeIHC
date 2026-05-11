#include "Texture.h"
#include "CommonValues.h"
#include <cstdio>

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}

Texture::Texture(const char* FileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = FileLoc;
}

bool Texture::LoadTextureA()
{
	if (fileLocation == nullptr)
	{
		printf("Ruta de textura nula.\n");
		return false;
	}

	stbi_set_flip_vertically_on_load(true);

	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, STBI_rgb_alpha);
	if (!texData)
	{
		printf("No se encontro el archivo o no pudo cargarse: %s\n", fileLocation);
		return false;
	}

	if (width <= 0 || height <= 0)
	{
		printf("Dimensiones invalidas en textura: %s\n", fileLocation);
		stbi_image_free(texData);
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;
}

bool Texture::LoadTexture()
{
	if (fileLocation == nullptr)
	{
		printf("Ruta de textura nula.\n");
		return false;
	}

	stbi_set_flip_vertically_on_load(true);

	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("No se encontro el archivo o no pudo cargarse: %s\n", fileLocation);
		return false;
	}

	if (width <= 0 || height <= 0)
	{
		printf("Dimensiones invalidas en textura: %s\n", fileLocation);
		stbi_image_free(texData);
		return false;
	}

	GLenum format = GL_RGB;
	if (bitDepth == 1) format = GL_RED;
	else if (bitDepth == 3) format = GL_RGB;
	else if (bitDepth == 4) format = GL_RGBA;
	else
	{
		printf("Formato de textura no soportado en: %s | canales: %d\n", fileLocation, bitDepth);
		stbi_image_free(texData);
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;
}

void Texture::ClearTexture()
{
	if (textureID != 0)
	{
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}

	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::~Texture()
{
	ClearTexture();
}