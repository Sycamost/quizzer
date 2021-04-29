#include "SweetieJar.h"
#include "QuestionType.h"
#include "util.h"

static_assert(false, "Before getting rid of this static assertion, make sure you tick off everything on the following checklist:\n"
	"x Move this file, SweetieJarReader.cpp and SweetieJarWriter.cpp to the correct filters (follow precedent).\n"
	"x Likewise with SweetieJar.h, SweetieJarReader.h and SweetieJarWriter.h.\n"
	"o Fill in the blanks in this file, SweetieJar.h, SweetieJarReader.cpp and SweetieJarWriter.cpp.\n"
	"x Make a new entry in the QuestionType enum class.\n"
	"o Append a new item to the QuestionTypeInfo list in QuestionTypeInfo.cpp.\n");

SweetieJar::SweetieJar(std::wstring question, long double number, long double accuracy, size_t decimalPoints, size_t leadingZeroes, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question + L" Answer accurate to within " + std::to_wstring(accuracy) + L"."),
	_number(number),
	_accuracy(accuracy),
	_decimalPoints(decimalPoints),
	_leadingZeroes(leadingZeroes),
	_minDigitsExponent(SIZE_MAX)
{}

SweetieJar::SweetieJar(std::wstring question, long double number, long double accuracy, size_t decimalPoints, size_t leadingZeroes, size_t minDigitsExponent, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question + L" Answer accurate to within " + std::to_wstring(accuracy) + L"."),
	_number(number),
	_accuracy(accuracy),
	_decimalPoints(decimalPoints),
	_leadingZeroes(leadingZeroes),
	_minDigitsExponent(minDigitsExponent)
{}

SweetieJar::SweetieJar(std::wstring question, long long number, long long accuracy, size_t decimalPoints, size_t leadingZeroes, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question + L" Answer accurate to within " + std::to_wstring(accuracy) + L"."),
	_number((long double)number),
	_accuracy((long double)accuracy),
	_decimalPoints(decimalPoints),
	_leadingZeroes(leadingZeroes),
	_minDigitsExponent(SIZE_MAX)
{}

SweetieJar::SweetieJar(std::wstring question, long long number, long long accuracy, size_t decimalPoints, size_t leadingZeroes, size_t minDigitsExponent, easy_list::list<std::wstring> tags) :
	Question(QuestionType::SWEETIE_JAR, tags),
	_question(question + L" Answer accurate to within " + std::to_wstring(accuracy) + L"."),
	_number((long double)number),
	_accuracy((long double)accuracy),
	_decimalPoints(decimalPoints),
	_leadingZeroes(leadingZeroes),
	_minDigitsExponent(minDigitsExponent)
{}

std::wstring SweetieJar::getQuestion()
{
	return _question;
}

std::wstring SweetieJar::getAnswer()
{
	return formatNumber(_number, SIZE_MAX, _leadingZeroes, _minDigitsExponent);
}

bool SweetieJar::isCorrect(std::wstring guess)
{
	auto cstr = std::string(guess.begin(), guess.end()).c_str();
	long double error = std::strtold(cstr, nullptr) - _number;
	error = (error >= 0 ? error : -error);
	return error <= _accuracy;
}

void SweetieJar::writeChildData(std::wofstream& stream)
{
	static_assert(false, "SweetieJar::writeChildData() has not been implemented yet!");
}