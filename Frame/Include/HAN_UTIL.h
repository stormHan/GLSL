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

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define SNPRINTF _snprintf_s

#define INVALID_UNIFORM_LOCATION 0xffffffff

bool ReadFile(const char* filename, string& outFile);

void FileReadingError(const char* pFileName, unsigned int line, const char* pError);

#define FILE_READING_ERROR(FileError) FileReadingError(__FILE__, __LINE__, FileError); 



#endif