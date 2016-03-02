#include <iostream>
#include <fstream>
#ifdef WIN32
#include <Windows.h>
#endif

#include "HAN_UTIL.h"

bool ReadFile(const char* pFilename, string& outFile)
{
	ifstream f(pFilename);

	bool ret = false;

	if (f.is_open()){
		string line;
		while (getline(f, line)){
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else{
		FILE_READING_ERROR(pFilename);
	}

	return ret;
}
void FileReadingError(const char* pFileName, unsigned int line, const char* pError)
{
#ifdef WIN32
	char msg[1024];
	_snprintf(msg, sizeof(msg), "%s:%d unable to open file '%s'", pFileName, line, *pError);
	MessageBoxA(NULL, msg, NULL, 0);
#else
	fprintf(stderr, "%s:%d: unable to open file `%s`\n", pFileName, line, pFileError);
#endif
}