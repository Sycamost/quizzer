#include "SweetieJar.h"
#include "QuestionType.h"
#include "util.h"

SweetieJar::SweetieJar(std::wstring question, long double number, long double accuracy, size_t decimalPoints, size_t leadingZeroes, bool displayAsExp, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question + L" Answer accurate to within " + std::to_wstring(accuracy) + L"."),
	_number(number),
	_accuracy(accuracy),
	_decimalPoints(decimalPoints),
	_leadingZeroes(leadingZeroes),
	_displayAsExp(displayAsExp)
{}

SweetieJar::SweetieJar(std::wstring question, long long number, long long accuracy, size_t decimalPoints, size_t leadingZeroes, bool displayAsExp, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question + L" Answer accurate to within " + std::to_wstring(accuracy) + L"."),
	_number((long double)number),
	_accuracy((long double)accuracy),
	_decimalPoints(decimalPoints),
	_leadingZeroes(leadingZeroes),
	_displayAsExp(displayAsExp)
{}

std::wstring SweetieJar::getQuestion()
{
	return _question;
}

std::wstring SweetieJar::getAnswer()
{
	return formatNumber(_number, SIZE_MAX, _leadingZeroes, 0);
}

bool SweetieJar::isCorrect(std::wstring guess)
{
	auto cstr = wstrToStr(guess).c_str();
	long double error = std::strtold(cstr, nullptr) - _number;
	error = (error >= 0 ? error : -error);
	return error <= _accuracy;
}

void SweetieJar::writeChildData(std::wofstream& stream)
{
	stream << _question << L"\n"
		<< _number << L"\n"
		<< _accuracy << L"\n"
		<< _decimalPoints << L"\n"
		<< _leadingZeroes << L"\n"
		<< _displayAsExp << L"\n";
}