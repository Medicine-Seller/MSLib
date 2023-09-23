#include "MSLib.h"

std::string ms::ReplaceString(std::string szStr, std::string szFind, std::string szReplace)
{
	while (szStr.find(szFind) != std::string::npos)
		szStr.replace(szStr.find(szFind), szReplace.length(), szReplace);

	return szStr;
}

std::vector<BYTE> ms::StringToBytes(std::string szBytes)
{
	std::istringstream iss(szBytes);
	std::vector<BYTE> vBytes;
	DWORD dwByte;

	while (iss >> std::hex >> dwByte)
		vBytes.push_back(dwByte);

	return vBytes;
}

std::vector<CHAR> ms::GenerateMask(std::string szSig)
{
	std::stringstream ss(szSig);
	std::string szStr;
	std::vector<CHAR> vMask;

	while (ss >> szStr)
	{
		if (szStr == "??" || szStr == "?")
			vMask.push_back('?');
		else
			vMask.push_back('x');
	}

	return vMask;
}