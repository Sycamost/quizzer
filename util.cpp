#include <iostream>
#include <string>
#include <math.h>
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

std::wstring formatNumber(const long long number, const size_t precision, const size_t leadingZeroes, const size_t minExp)
{
	return formatNumber((long double)number, precision, leadingZeroes, minExp);
}

/// <summary>
/// Returns the specified digit in base 10 of the given number.
/// </summary>
/// <param name="number">The number</param>
/// <param name="digit">The zero-indexed digit</param>
std::wstring getDigit(const long double number, size_t digit)
{
	long double result = number / std::powl(10, digit - 1);
	result -= std::remainderl(result, 1);
	return std::to_wstring(result);
}

std::wstring formatNumber(const long double number, const size_t precision, const size_t leadingZeroes, const size_t minExp)
{
	size_t numDigits = 0;
	for (long double quotient = number; quotient >= 1.0; quotient /= 10.0)
		numDigits++;

	// Plonk on leading zeroes
	std::wstring result = std::wstring(std::max(leadingZeroes, numDigits) - numDigits, L'0');

	// Do exponents only if no leading zeroes were specified and we have enough digits
	if (leadingZeroes == 0 && numDigits >= minExp)
	{
		// Do exponent - pretend this number is smaller than it is, format it, then add the exponent string on the end.
		return formatNumber(number / (std::powl(10, numDigits - 1)), 0, precision) + L"e" + std::to_wstring(numDigits - 1);
	}

	// Make it just precise enough
	for (size_t i = 0; i < precision; i++)
	{
		if (i == numDigits)
			result.push_back(L'.');
		result += getDigit(number, i);
	}

	return result;
}

std::string wstrToStr(std::wstring wstr)
{
	return std::string(wstr.begin(), wstr.end());
}

std::wstring strToWstr(std::string str)
{
	return std::wstring(str.begin(), str.end());
}
