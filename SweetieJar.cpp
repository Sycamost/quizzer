#include "SweetieJar.h"
#include "QuestionType.h"
#include "util.h"

SweetieJar::SweetieJar(std::wstring question, long double number, long double accuracy, size_t sigFigs, size_t leadingZeroes, bool displayAsExp, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question),
	_number(number),
	_accuracy(accuracy),
	_sigFigs(sigFigs),
	_leadingZeroes(leadingZeroes),
	_displayAsExp(displayAsExp)
{}

SweetieJar::SweetieJar(std::wstring question, long long number, long long accuracy, size_t sigFigs, size_t leadingZeroes, bool displayAsExp, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question),
	_number((long double)number),
	_accuracy((long double)accuracy),
	_sigFigs(sigFigs),
	_leadingZeroes(leadingZeroes),
	_displayAsExp(displayAsExp)
{}

std::wstring SweetieJar::getQuestion()
{
	const std::wstring fullQuestion = _question + L" Answer accurate to within " + formatNumber(_accuracy, _sigFigs, _leadingZeroes, _displayAsExp ? 0 : SIZE_MAX) + L".";
	return fullQuestion;
}

std::wstring SweetieJar::getAnswer()
{
	return formatNumber(_number, _sigFigs, _leadingZeroes, _displayAsExp ? 0 : SIZE_MAX);
}

bool SweetieJar::isCorrect(std::wstring guess)
{
	auto cstr = new char;
	std::strcpy(cstr, wstrToStr(guess).c_str());
	long double error = std::strtold(cstr, nullptr) - _number;
	error = (error >= 0 ? error : -error);
	return error <= _accuracy;
}

void SweetieJar::writeChildData(std::wofstream& stream)
{
	stream << _question << L"\n"
		<< _number << L"\n"
		<< _accuracy << L"\n"
		<< _sigFigs << L"\n"
		<< _leadingZeroes << L"\n"
		<< _displayAsExp << L"\n";
}