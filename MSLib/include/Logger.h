#include <string>

#ifndef MSLOGGER_H
#define MSLOGGER_H

namespace ms
{
	void Log(std::string fileDirectory, unsigned int line, const char* format, ...);
}

#define LogLine(x, ...) ms::Log(__FILE__, __LINE__, x, __VA_ARGS__);
#define LogFunction(x, ...) printf("%s: " x, __FUNCTION__, __VA_ARGS__)

#endif