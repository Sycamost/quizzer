#include <iostream>
#include <string>
#include "util.h"

std::wstring getUserInputLine(std::wistream& stream)
{
	std::wstring userInput = L"";
	std::getline(stream, userInput);
	return userInput;
}

bool getUserYesNo()
{
	return isYes(getUserInputLine());
}

std::wstring toUpper(std::wstring wstr)
{
	std::wstring result = L"";
	for (unsigned int i = 0; i < wstr.length(); i++) {
		result.push_back(std::toupper(wstr[i]));
	}
	return result;
}

std::wstring toLower(std::wstring wstr)
{
	std::wstring result = L"";
	for (unsigned int i = 0; i < wstr.length(); i++) {
		result.push_back(std::tolower(wstr[i]));
	}
	return result;
}

bool isYes(std::wstring str)
{
	str = toUpper(str);
	return (str == L"Y" || str == L"YES" || str == L"1" || str == L"TRUE");
}

template<typename T> inline bool shareAnyElems(std::vector<T> v1, std::vector<T> v2)
{
	for (int i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < v2.size(); j++)
		{
			if (v1[i] == v2[j])
				return true;
		}
	}
	return false;
}