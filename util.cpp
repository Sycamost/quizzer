#include <string>
#include "util.h"

std::wstring toUpper(std::wstring wstr)
{
	std::wstring result = L"";
	for (int i = 0; i < wstr.length(); i++) {
		result.push_back(std::toupper(wstr[i]));
	}
}
