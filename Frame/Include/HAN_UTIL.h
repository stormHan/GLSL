/*


���Լ�д��һ��ͼ��ѧͨ�ÿ⣬�����ڸ�����ȡshader�Լ�һЩ��������


���Լ�д��һ��ͼ��ѧͨ�ÿ⣬�����ڸ�����ȡshader�Լ�һЩ��������

*/

#ifndef H_HAN_UTIL_H
#define H_HAN_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265f
#endif

bool ReadFile(const char* filename, string& outFile);

void FileReadingError(const char* pFileName, unsigned int line, const char* pError);

#define FILE_READING_ERROR(FileError) FileReadingError(__FILE__, __LINE__, FileError); 



#endif