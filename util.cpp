#include <iostream>
#include <string>
#include <math.h>
#include <map>
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

int getFirstDigitIndex(const long double number)
{
	if (number < 0.L)
		return getFirstDigitIndex(-number);

	if (number == 0.L)
		return 0;

	long double quotient = number;
	int firstDigitIndex;
	if (number < 1.L)
	{
		for (firstDigitIndex = 0; quotient < 1.L; firstDigitIndex--)
			quotient *= 10;
	}
	else
	{
		for (firstDigitIndex = 0; quotient >= 10.L; firstDigitIndex++)
			quotient /= 10;
	}

	return firstDigitIndex;
}

int getLastDigitIndex(const long double number)
{
	if (number < 0.L)
		return getLastDigitIndex(-number);

	if (number == 1.L)
	{
		return 0;
	}

	int digitIndex = 0;

	if (number > 1.L)
	{
		while (number != std::fmodl(number, std::powl(10.L, digitIndex)))
			digitIndex++;
	}

	else // number < 1.L
	{
		while (number == std::fmodl(number, std::powl(10.L, digitIndex)))
			digitIndex--;
	}

	return digitIndex;
}

/// <summary>
/// Returns the specified digit in base 10 of the given number.
/// </summary>
/// <param name="number">The number</param>
/// <param name="offset">The number of digits after the first digit to retrieve</param>
/// <example>
/// getDigit(1234.56789, 4) returns 5
/// </example>
wchar_t getDigit(const long double number, size_t offset)
{
	if (number < 0.L)
		return getDigit(-number, offset);

	// Find the index of the first digit and store it, since
	// getDigit is usually called many times on the one number
	static std::map<long double, int> numberFirstDigits;
	int firstDigit;
	auto iter = numberFirstDigits.find(number);
	if (iter == numberFirstDigits.end())
	{
		firstDigit = getFirstDigitIndex(number);
		numberFirstDigits.insert({ number, firstDigit });
	}
	else
	{
		firstDigit = iter->second;
	}

	int digitIndex = firstDigit - (int)offset;
	long double digitAndAfter = std::fmodl(number / std::powl(10, digitIndex), 10.L);
	long long digit = std::llroundl(digitAndAfter - std::fmodl(digitAndAfter, 1.L));
	return std::to_wstring(digit).front();
}

std::wstring formatIntegerSigFigs(const long long number, const size_t sigFigs, const size_t leadingZeroes, const size_t minExp)
{
	auto formattedString = formatNumberSigFigs((long double)number, sigFigs, leadingZeroes, minExp);
	return formattedString.substr(0, formattedString.find_first_of(L'.'));
}

std::wstring formatNumberSigFigs(const long double number, const size_t sigFigs, const size_t leadingZeroes, const size_t minExp)
{
	if (number < 0.L)
		return L"-" + formatNumberSigFigs(-number, sigFigs, leadingZeroes, minExp);

	int firstDigitIndex = getFirstDigitIndex(number);

	if (sigFigs == SIZE_MAX)
		return formatNumberSigFigs(number, firstDigitIndex - getLastDigitIndex(number), leadingZeroes, minExp);

	// Do exponents only if no leading zeroes were specified and we would have an exponent bigger than minExp
	if (leadingZeroes == 0 && std::abs(firstDigitIndex) >= minExp)
	{
		// Do exponent - pretend this number is smaller than it is, format it, then add the exponent string on the end.
		return formatNumberSigFigs(number / std::powl(10, firstDigitIndex), sigFigs) + L"e" + std::to_wstring(firstDigitIndex);
	}

	std::wstring result = L"";

	// Round, and write just enough significant figures
	int exp = firstDigitIndex + 1 - sigFigs;
	long double sigExpFactor = std::powl(10.L, exp);
	long double roundedNumber = number - std::fmodl(number, sigExpFactor);
	switch (getDigit(number, sigFigs))
	{
	case L'5':
	case L'6':
	case L'7':
	case L'8':
	case L'9':
		roundedNumber = std::ceill(roundedNumber / sigExpFactor) * sigExpFactor;
		break;
	default:
		roundedNumber = std::floorl(roundedNumber / sigExpFactor) * sigExpFactor;
		break;
	}
	for (size_t i = 0; i < sigFigs; i++)
		result += getDigit(roundedNumber, i);

	// Append zeroes as necessary after the significant figures
	result.insert(result.size(), std::wstring(std::min(0uLL, firstDigitIndex + 1 - result.length()), L'0'));

	// Decimal point?
	// Special case - zeroes after the decimal point, i.e. 0.0[...]0ABC[...]
	if (firstDigitIndex < 0)
		result.insert(0, L"0." + std::wstring(-firstDigitIndex - 1, L'0'));
	// Otherwise, just need to insert a decimal point in the right place - i.e. turn 12345 into 123.45
	else if ((long int)sigFigs > (long int)firstDigitIndex + 1)
		result.insert(firstDigitIndex + 1, L".");

	// Leading zeroes
	std::wstring leadingDigits = result.substr(0, leadingZeroes);
	leadingDigits = leadingDigits.substr(0, leadingDigits.find(L'.'));
	result.insert(0, std::wstring(leadingZeroes - leadingDigits.length(), L'0'));

	return result;
}

std::wstring formatNumberDecimalPoints(const long double number, const size_t decimalPoints, const size_t leadingZeroes, const size_t minExp)
{
	if (number < 0.L)
		return L"-" + formatNumberDecimalPoints(-number, decimalPoints, leadingZeroes, minExp);

	int firstDigitIndex = getFirstDigitIndex(number);

	if (decimalPoints == SIZE_MAX)
		return formatNumberSigFigs(number, firstDigitIndex - getLastDigitIndex(number), leadingZeroes, minExp);

	// First, count the number of significant figures we'll need. Then format it by significant figures
	int sigFigs = firstDigitIndex + decimalPoints + 1;
	return formatNumberSigFigs(number, sigFigs, leadingZeroes, minExp);
}

std::string wstrToStr(std::wstring wstr)
{
	return std::string(wstr.begin(), wstr.end());
}

std::wstring strToWstr(std::string str)
{
	return std::wstring(str.begin(), str.end());
}

bool interpretLongDouble(std::wstring input, long double* result)
{
	char* cstrLine = new char;
	std::strcpy(cstrLine, wstrToStr(input).c_str());
	char** strEnd = new char*;
	*result = std::strtold(cstrLine, strEnd);
	return *result != HUGE_VALL && *strEnd != cstrLine;
}

bool interpretSize(std::wstring input, size_t* result)
{
	char* cstrLine = new char;
	std::strcpy(cstrLine, wstrToStr(input).c_str());
	char** strEnd = new char*;
	auto ulResult = std::strtoul(cstrLine, strEnd, 10);

	if (ulResult > (unsigned long)SIZE_MAX)
		*result = SIZE_MAX;
	else
		*result = static_cast<size_t>(ulResult);

	return *strEnd != cstrLine;
}

bool isNumericalWchar(wchar_t wch)
{
	static const auto numberWchars = easy_list::list<wchar_t>({ L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9' });
	return numberWchars.contains(wch);
}

easy_list::list<std::wstring> getExponentStringList()
{
	static auto list = easy_list::list<std::wstring>({ L"e", L"e+", L"exp", L"Exp", L"E", L"E+", L"EXP", L"x10^", L"x 10^", L"*^", L"⏨" });
	return easy_list::list<std::wstring>(list);
}

bool isExponentString(std::wstring wstr)
{
	return getExponentStringList().contains(wstr);
}

[[nodiscard]] std::wstring removeExponentString(std::wstring wstr)
{
	return wstr.substr(0, getExponentStringList().transform<size_t>([wstr](std::wstring expStr) -> size_t { return wstr.find(expStr); }).min());
}

size_t countSignificantFigures(std::wstring input)
{
	if (input.empty())
		return 0;

	if (input[0] == L'-')
		return countSignificantFigures(input.substr(1));

	input = removeExponentString(input);

	// First, count every digit from the first non-zero digit
	size_t count = 0ull;
	auto inputList = easy_list::list<wchar_t>(input);
	bool leadingZero = true;
	for (size_t i = 0; i < input.size(); i++)
	{
		// If it's a number, add to the count and continue
		if (isNumericalWchar(input[i]))
		{
			if (input[i] != L'0')
				leadingZero = false;
			if (!leadingZero)
				count++;
			continue;
		}

		// Is it an exponent string here? If so, we're finished.
		bool isExp = false;
		for (size_t len = 1; i + len <= input.size(); len++)
		{
			if (isExponentString(inputList.slice(i, len)))
				return count;
		}

		// Otherwise shut our eyes and hope nothing's wrong. It could be a decimal point.
		// (This function assumes that we are indeed passed a valid number string.)
	}

	// Now, subtract any trailing zeroes
	for (size_t i = input.size() - 1; i < input.size(); i--)
	{
		switch (input[i])
		{
		case L'0':
			if (count == 0ull)
				return 0ull;	// Catches the case where all the digits are trailing zeroes
			count--;
			break;
		case L'1':
		case L'2':
		case L'3':
		case L'4':
		case L'5':
		case L'6':
		case L'7':
		case L'8':
		case L'9':
			i = -1;
			break;
		default:
			// Could be e.g. a decimal point
			break;
		}
	}

	return count;
}

size_t countDecimalPoints(std::wstring input)
{
	input = removeExponentString(input);
	auto decimalPointIndex = input.find_first_of(L'.');
	if (decimalPointIndex == input.npos)
		return 0;
	return input.size() - decimalPointIndex;
}

size_t countLeadingZeroes(std::wstring input)
{
	if (input.empty())
		return 0;

	if (input[0] == L'-')
		return countLeadingZeroes(input.substr(1));

	// Special case - a fraction less than 1
	if (input.substr(0, 2) == L"0.")
		return 0;

	size_t count = 0;
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == L'0')
			count++;
		else
			break;
	}
	return count;
}
