#include "PGEngine.h"

std::wstring strconv(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

std::string strconv(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};