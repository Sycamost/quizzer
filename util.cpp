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

std::wstring indent(std::wstring wstr, int numTabs)
{
	wstr = L'\t' + wstr;
	for (unsigned int i = 0; i < wstr.length(); i++)
	{
		if (wstr[i] == L'\n')
		{
			i++;
			wstr = wstr.substr(0, i) + L'\t' + wstr.substr(i, wstr.length());
		}
	}
	return wstr;
}

bool isYes(std::wstring str)
{
	str = toUpper(str);
	return (str == L"Y" || str == L"YES" || str == L"1" || str == L"TRUE");
}