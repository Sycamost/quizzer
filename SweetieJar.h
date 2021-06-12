#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include "Question.h"

class SweetieJar : public Question
{
private:
	size_t _questionLength;
	std::wstring _questionMsg;
	long double _number;
	std::wstring _numberMsg;
	long double _accuracy;
	size_t _sigFigsOrDecimalPoints;
	size_t _leadingZeroes;
	bool _displayAsExp;
	bool _decimalPoints;
	void writeChildData(std::wofstream& stream);
public:
	SweetieJar(std::wstring question, long double number, long double accuracy, size_t sigFigsOrDecimalPoints, size_t leadingZeroes, bool displayAsExp, bool decimalPoints = false, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	SweetieJar(std::wstring question, long long number, long long accuracy, size_t sigFigsOrDecimalPoints, size_t leadingZeroes, bool displayAsExp, bool decimalPoints = false, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	std::wstring getQuestion();
	std::wstring getAnswer();
	bool isCorrect(std::wstring guess);
};