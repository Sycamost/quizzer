#include "TrueOrFalse.h"
#include "QuestionType.h"
#include "util.h"

TrueOrFalse::TrueOrFalse(
	std::wstring prompt,
	bool isTrue,
	easy_list::list<std::wstring> tags) :
	Question(QuestionType::TRUE_OR_FALSE, tags),
	_prompt(L"True or false: " + prompt),
	_isTrue(isTrue)
{
}

std::wstring TrueOrFalse::getQuestion()
{
	return _prompt;
}

std::wstring TrueOrFalse::getAnswer()
{
	return _isTrue ? L"true" : L"false";
}

bool TrueOrFalse::isCorrect(std::wstring guess)
{
	return getYesNo(guess) && _isTrue || !getYesNo(guess) && !_isTrue;
}

void TrueOrFalse::writeChildData(std::wofstream& stream)
{
	stream << _prompt << L"";
	stream << _isTrue << L"";

}