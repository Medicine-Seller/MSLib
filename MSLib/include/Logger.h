#include <string>

#ifndef MSLOGGER_H
#define MSLOGGER_H

namespace ms
{
	void Log(const std::string fileDirectory, const unsigned int line, const char* format, ...);
}

#define LOG_LINE(...) ms::Log(__FILE__, __LINE__, __VA_ARGS__)

#endif