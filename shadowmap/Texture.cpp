#include "Texture.h"


Texture::Texture()
{
}

AUX_RGBImageRec* Texture::LoadBMP(CHAR *Filename)    // ����λͼͼ��
{
	FILE *File = NULL;         // �ļ����
	if (!Filename)          // ȷ���ļ������ṩ
	{
		return NULL;         // ���û�ṩ������ NULL
	}
	File = fopen(Filename, "r");       // ���Դ��ļ�
	if (File)           // �ļ�����ô?
	{
		fclose(File);         // �رվ��
		return auxDIBImageLoadA(Filename);    // ����λͼ������ָ��
	}
	return NULL;          // �������ʧ�ܣ����� NULL
}

int Texture::loadBMPTexture(GLuint  texture[], CHAR *filename)
{
	int Status = FALSE;         // ״ָ̬ʾ��
	AUX_RGBImageRec *TextureImage[1];     // ��������Ĵ洢�ռ�
	memset(TextureImage, 0, sizeof(void *) * 1);   // ��ָ����Ϊ NULL
	// ����λͼ��������޴������λͼû�ҵ����˳�
	if (TextureImage[0] = LoadBMP(filename))
	{
		Status = TRUE;         // �� Status ��Ϊ TRUE
		glGenTextures(1, &texture[0]);     // ��������

		// ʹ������λͼ�������� �ĵ�������
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// ��������
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // �����˲�
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // �����˲�
	}

	if (TextureImage[0])        // �����Ƿ����
	{
		if (TextureImage[0]->data)      // ����ͼ���Ƿ����
		{
			free(TextureImage[0]->data);    // �ͷ�����ͼ��ռ�õ��ڴ�
		}
		else printf("the texture image is not existing!");

		free(TextureImage[0]);       // �ͷ�ͼ��ṹ
	}
	else printf("the texture is not existing!");
	return Status;          // ���� Status
}

bool Texture::loadTGATexture(char *filename)
{
	GLubyte     TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };    // Uncompressed TGA Header  
	GLubyte     TGAcompare[12];                             // Used To Compare TGA Header  
	GLubyte     header[6];                                  // First 6 Useful Bytes From The Header  
	GLuint      bytesPerPixel;                              // Holds Number Of Bytes Per Pixel Used In The TGA File  
	GLuint      imageSize;                                  // Used To Store The Image Size When Setting Aside Ram  
	GLuint      temp;                                       // Temporary Variable  
	GLuint      type = GL_RGBA;                               // Set The Default GL Mode To RBGA (32 BPP)  

	FILE *file = fopen(filename, "rb");                     // Open The TGA File  

	if (file == NULL ||                                       // Does File Even Exist?  
		fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||  // Are There 12 Bytes To Read?  
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||  // Does The Header Match What We Want?  
		fread(header, 1, sizeof(header), file) != sizeof(header))                // If So Read Next 6 Header Bytes  
	{
		if (file == NULL)                                   // Did The File Even Exist? *Added Jim Strong*  
			return false;                                   // Return False  
		else
		{
			fclose(file);                                   // If Anything Failed, Close The File  
			return false;                                   // Return False  
		}
	}

	this->width = header[1] * 256 + header[0];           // Determine The TGA Width  (highbyte*256+lowbyte)
	this->height = header[3] * 256 + header[2];           // Determine The TGA Height (highbyte*256+lowbyte)

	//OpenGL������ֻ��ʹ��24λ����32λ��TGAͼ��  
	if (this->width <= 0 ||                              // Is The Width Less Than Or Equal To Zero  
		this->height <= 0 ||                              // Is The Height Less Than Or Equal To Zero  
		(header[4] != 24 && header[4] != 32))                   // Is The TGA 24 or 32 Bit?  
	{
		fclose(file);                                       // If Anything Failed, Close The File  
		return false;                                       // Return False  
	}

	this->bpp = header[4];                            // Grab The TGA's Bits Per Pixel (24 or 32)  
	bytesPerPixel = this->bpp / 8;                        // Divide By 8 To Get The Bytes Per Pixel  
	imageSize = this->width*this->height*bytesPerPixel;   // Calculate The Memory Required For The TGA Data  

	this->imageData = (GLubyte *)malloc(imageSize);     // Reserve Memory To Hold The TGA Data  

	if (this->imageData == NULL ||                          // Does The Storage Memory Exist?  
		fread(this->imageData, 1, imageSize, file) != imageSize)    // Does The Image Size Match The Memory Reserved?  
	{
		if (this->imageData != NULL)                     // Was Image Data Loaded  
			free(this->imageData);                        // If So, Release The Image Data  

		fclose(file);                                       // Close The File  
		return false;                                       // Return False  
	}

	//RGB���ݸ�ʽת����������OpenGL��ʹ��  
	for (GLuint i = 0; i<int(imageSize); i += bytesPerPixel)      // Loop Through The Image Data  
	{                                                       // Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)  
		temp = this->imageData[i];                          // Temporarily Store The Value At Image Data 'i'  
		this->imageData[i] = this->imageData[i + 2];    // Set The 1st Byte To The Value Of The 3rd Byte  
		this->imageData[i + 2] = temp;                    // Set The 3rd Byte To The Value In 'temp' (1st Byte Value)  
	}

	// Build A Texture From The Data  
	glGenTextures(1, &this->texID);                    // Generate OpenGL texture IDs  

	glBindTexture(GL_TEXTURE_2D, this->texID);         // Bind Our Texture  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // Linear Filtered  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // Linear Filtered  

	if (this->bpp == 24)                                 // Was The TGA 24 Bits  
	{
		type = GL_RGB;                                        // If So Set The 'type' To GL_RGB  
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, this->width, this->height, 0, type, GL_UNSIGNED_BYTE, this->imageData);

	return true;
}

void Texture::setTexParament()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
}
