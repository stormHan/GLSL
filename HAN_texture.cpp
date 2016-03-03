//---------------------------------------
//
//			stormhan  texture.cpp
//
//		a class used to load texture
//--------------------------------------
#include <iostream>
#include  "HAN_texture.h"

Texture::Texture(GLenum TextureTarget, const std::string Filename)
{
	m_filename = Filename;
	m_textureTarget = TextureTarget;
}

bool Texture::Load()
{
	try{
		m_image.read(m_filename);
		m_image.write(&m_blob, "RGBA");
	}
	catch (Magick::Error& Error){
		std::cout << "Error loading texture '" << m_filename << " ' " <<  Error.what() << std::endl;
		return false;
	}

	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, GL_RGBA, m_image.columns(), m_image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(m_textureTarget, 0);

	return true;
}

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}