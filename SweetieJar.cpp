#include "SweetieJar.h"
#include "QuestionType.h"
#include "util.h"

SweetieJar::SweetieJar(std::wstring question, long double number, long double accuracy, size_t sigFigsOrDecimalPoints, size_t leadingZeroes, bool displayAsExp, bool decimalPoints, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question),
	_number(number),
	_accuracy(accuracy),
	_sigFigsOrDecimalPoints(sigFigsOrDecimalPoints),
	_leadingZeroes(leadingZeroes),
	_displayAsExp(displayAsExp),
	_decimalPoints(decimalPoints)
{}

SweetieJar::SweetieJar(std::wstring question, long long number, long long accuracy, size_t sigFigsOrDecimalPoints, size_t leadingZeroes, bool displayAsExp, bool decimalPoints, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question),
	_number((long double)number),
	_accuracy((long double)accuracy),
	_sigFigsOrDecimalPoints(sigFigsOrDecimalPoints),
	_leadingZeroes(leadingZeroes),
	_displayAsExp(displayAsExp),
	_decimalPoints(decimalPoints)
{}

std::wstring SweetieJar::getQuestion()
{
	static std::wstring fullQuestion = _question + (
		_accuracy == 0.L ?
			L"Answer precisely." :
			(L" Answer accurate to within " +
				(_decimalPoints ?
					formatNumberDecimalPoints(_accuracy, _sigFigsOrDecimalPoints, _leadingZeroes, _displayAsExp ? 0 : SIZE_MAX) :
					formatNumberSigFigs(_accuracy, _sigFigsOrDecimalPoints, _leadingZeroes, _displayAsExp ? 0 : SIZE_MAX))
				+ L".")
	);
	return fullQuestion;
}

std::wstring SweetieJar::getAnswer()
{
	static std::wstring answer =
		_decimalPoints ?
			formatNumberDecimalPoints(_number, _sigFigsOrDecimalPoints, _leadingZeroes, _displayAsExp ? 0 : SIZE_MAX) :
			formatNumberSigFigs(_number, _sigFigsOrDecimalPoints, _leadingZeroes, _displayAsExp ? 0 : SIZE_MAX);
	return answer;
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
		<< _sigFigsOrDecimalPoints << L"\n"
		<< _leadingZeroes << L"\n"
		<< _displayAsExp << L"\n"
		<< _decimalPoints << L"\n";
}