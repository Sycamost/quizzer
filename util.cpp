#include <iostream>
#include <string>
#include "util.h"
#include "Command.h"

const YesNo YesNo::YES{ YesNo(YesNo::Value::YES) };
const YesNo YesNo::NO{ YesNo(YesNo::Value::NO) };
const YesNo YesNo::COMMAND{ YesNo(YesNo::Value::COMMAND) };
const YesNo YesNo::OTHER{ YesNo(YesNo::Value::OTHER) };

std::wstring getInputLine(std::wistream& stream)
{
	std::wstring userInput = L"";
	std::getline(stream, userInput);
	return userInput;
}

bool inputYesNo(std::wstring message, bool doPrintResult)
{
	message += L" [Y/N]\n";
	const YesNo* result;
	while (true)
	{
		std::wcout << message;

		result = &getYesNo(getInputLine());

		if (*result)
		{
			if (doPrintResult)
				std::wcout << L"I interpreted that as a \"yes\".\n";
			return true;
		}

		else if (!*result)
		{
			if (doPrintResult)
				std::wcout << L"I interpreted that as a \"no\".\n";
			return false;
		}

		else if (*result == YesNo::COMMAND)
		{
			std::wcout << L"Please answer this question before giving me further commands.\n";
			continue;
		}

		else
		{
			std::wcout << L"Please enter either \"yes\" or \"no\".\n";
			continue;
		}
	}
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
	str = toLower(str);
	if (str == L"y" || str == L"yes" || str == L"1" || str == L"T" || str == L"true")
		return YesNo::YES;
	if (str == L"n" || str == L"no" || str == L"0" || str == L"F" || str == L"false")
		return YesNo::NO;
	if (!Command::makePossibleCommands(str).empty())
		return YesNo::COMMAND;
	return YesNo::OTHER;
}

easy_list::list<std::wstring> splitByWord(std::wstring wstr)
{
	easy_list::list<std::wstring> result = easy_list::list<std::wstring>();

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

easy_list::list<std::wstring> splitByWordOrQuotes(std::wstring wstr)
{
	easy_list::list<std::wstring> result = easy_list::list<std::wstring>();

	std::wstring currentStr = L"";
	bool waitingForEndQuote = false;
	for (unsigned int i = 0; i < wstr.length(); i++)
	{
		if (wstr[i] == L'\"')
		{
			if (waitingForEndQuote)
			{
				result.push_back(currentStr);
				currentStr = L"";
				waitingForEndQuote = false;
			}
			else
				waitingForEndQuote = true;
			continue;
		}

		else if (wstr[i] == L' ')
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
