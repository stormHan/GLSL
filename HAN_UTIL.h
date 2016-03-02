/*

���Լ�д��һ��ͼ��ѧͨ�ÿ⣬�����ڸ�����ȡshader�Լ�һЩ��������

*/

#ifndef H_HAN_UTIL_H
#define H_HAN_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

bool ReadFile(const char* filename, string& outFile);

void FileReadingError(const char* pFileName, unsigned int line, const char* pError);

#define FILE_READING_ERROR(FileError) FileReadingError(__FILE__, __LINE__, FileError); 



#endif