#pragma once
#include <string>
#include <iostream>
#include <easy_list.h>
#include <type_traits>
#include <sstream>
#include <locale>

class YesNo {
public:
	static const YesNo YES;
	static const YesNo NO;
	static const YesNo COMMAND;
	static const YesNo OTHER;
	operator bool() const { return _value == Value::YES; }
	bool operator!() const { return _value == Value::NO; }
	bool operator==(const YesNo& other) const { return _value == other._value; }
private:
	enum class Value {
		YES,
		NO,
		COMMAND,
		OTHER
	};
	Value _value;
	YesNo(Value value) { _value = value; }
};
std::wstring getInputLine(std::wistream &stream = std::wcin);
bool inputYesNo(std::wstring message, bool doPrintResult = true);
std::wstring toLower(std::wstring wstr);
std::wstring indent(std::wstring wstr, int numTabs);
const YesNo getYesNo(std::wstring wstr);
easy_list::list<std::wstring> splitByWord(std::wstring wstr);
std::wstring formatNumberSigFigs(const long long number, const size_t sigFigs = SIZE_MAX, const size_t leadingZeroes = 0, const size_t minExp = SIZE_MAX);
std::wstring formatNumberSigFigs(const long double number, const size_t sigFigs = SIZE_MAX, const size_t leadingZeroes = 0, const size_t minExp = SIZE_MAX);
std::wstring formatNumberDecimalPoints(const long double number, const size_t decimalPoints = -1, const size_t leadingZeroes = 0, const size_t minExp = SIZE_MAX);
std::string wstrToStr(std::wstring);
std::wstring strToWstr(std::string);
bool interpretLongDouble(std::wstring input, long double* result);
bool interpretSize(std::wstring input, size_t* result);
bool isNumericalWchar(wchar_t wch);
bool isExponentString(std::wstring wstr);
size_t countSignificantFigures(std::wstring input);
size_t countDecimalPoints(std::wstring input);
size_t countLeadingZeroes(std::wstring input);