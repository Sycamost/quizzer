#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include "Question.h"

class SweetieJar : public Question
{
private:
	std::wstring _question;
	long double _number;
	long double _accuracy;
	size_t _decimalPoints;
	size_t _leadingZeroes;
	size_t _minDigitsExponent;
	void writeChildData(std::wofstream& stream);
public:
	SweetieJar(std::wstring question, long double number, long double accuracy, size_t decimalPoints, size_t leadingZeroes, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	SweetieJar(std::wstring question, long double number, long double accuracy, size_t decimalPoints, size_t leadingZeroes, size_t minDigitsExponent, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	SweetieJar(std::wstring question, long long number, long long accuracy, size_t decimalPoints, size_t leadingZeroes, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	SweetieJar(std::wstring question, long long number, long long accuracy, size_t decimalPoints, size_t leadingZeroes, size_t minDigitsExponent, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	std::wstring getQuestion();
	std::wstring getAnswer();
	bool isCorrect(std::wstring guess);
};