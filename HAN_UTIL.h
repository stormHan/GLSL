/*

我自己写的一个图形学通用库，可用于辅助读取shader以及一些其它功能

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