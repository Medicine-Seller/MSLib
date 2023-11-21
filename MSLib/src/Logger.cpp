#include "Logger.h"

void ms::Log(std::string fileDirectory, unsigned int line, const char* format, ...)
{
	std::string fileName;
	size_t found = fileDirectory.find_last_of("/\\");
	if (found != std::string::npos)
		fileName = fileDirectory.substr(found + 1);

	printf("[%s] Line %d: ", fileName.c_str(), line);

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}