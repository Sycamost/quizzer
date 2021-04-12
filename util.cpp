#include <iostream>
#include <string>
#include "util.h"

const YesNo YesNo::YES{ YesNo(YesNo::Value::YES) };
const YesNo YesNo::NO{ YesNo(YesNo::Value::NO) };
const YesNo YesNo::INVALID{ YesNo(YesNo::Value::INVALID) };

std::wstring getInputLine(std::wistream& stream)
{
	std::wstring userInput = L"";
	std::getline(stream, userInput);
	return userInput;
}

bool inputYesNo(std::wstring message, bool doPrintResult)
{
	message += L" [Y/N]\n";
	YesNo result = YesNo::INVALID;
	while (true)
	{
		std::wcout << message;

		result = getYesNo(getInputLine());

		if (result)
		{
			if (doPrintResult)
				std::wcout << L"I interpreted that as a \"yes\".\n";
			return true;
		}

		if (!result)
		{
			if (doPrintResult)
				std::wcout << L"I interpreted that as a \"no\".\n";
			return false;
		}

		std::wcout << L"Please enter either \"yes\" or \"no\".\n";
	}
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

const YesNo getYesNo(std::wstring str)
{
	str = toUpper(str);
	if (str == L"Y" || str == L"YES" || str == L"1" || str == L"T" || str == L"TRUE")
		return YesNo::YES;
	if (str == L"N" || str == L"NO" || str == L"0" || str == L"F" || str == L"FALSE")
		return YesNo::NO;
	return YesNo::INVALID;
}

std::vector<std::wstring> splitByWord(std::wstring wstr)
{
	std::vector<std::wstring> result = std::vector<std::wstring>();

	std::wstring currentStr = L"";
	for (unsigned int i = 0; i < wstr.length(); i++)
	{
		if (wstr[i] == L' ')
		{
			if (!currentStr.empty())
			{
				result.push_back(currentStr);
				currentStr = L"";
			}
			continue;
		}
		currentStr.push_back(wstr[i]);
	}
	if (!currentStr.empty())
	{
		result.push_back(currentStr);
		currentStr = L"";
	}

	return result;
}
