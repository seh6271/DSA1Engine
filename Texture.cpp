#include "Texture.h"



Texture::Texture()
{
}


Texture::~Texture()
{
}


void Texture::load(char* texFile)
{
	//load texture	
	FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(texFile, 0), texFile);

	//test if load worked
	if (image == nullptr)
	{
		//load failed
		return;
	}

	//convert to 32bit bmp
	FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(image);
	//unload original
	FreeImage_Unload(image);

	//generate and bind a new texture
	
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//upload texture bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit), FreeImage_GetHeight(image32Bit), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32Bit));

	//set min filter to linear instead of mipmap linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	

	//clear the texture from RAM
	FreeImage_Unload(image32Bit);

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::unload()
{
	glDeleteTextures(1, &texID);
}

// render with a texture by binding it before glDraw call
void Texture::use() {
	glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::unbind()
{
	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}
