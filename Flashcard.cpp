#include <algorithm>
#include "Flashcard.h"
#include "globals.h"
#include "util.h"
#include "Option.h"

Flashcard::Flashcard(std::wstring question, std::wstring answer, bool caseSensitive, easy_list::list<std::wstring> tags) :
	Question(QuestionType::FLASHCARD, tags),
	_question(question),
	_answer(answer),
	_caseSensitive(caseSensitive)
{
}

std::wstring Flashcard::getQuestion()
{
	return _question;
}

std::wstring Flashcard::getAnswer()
{
	return _answer;
}

bool Flashcard::isCorrect(std::wstring guess)
{
	return _caseSensitive ? (guess == _answer) : (toLower(guess) == toLower(_answer));
}

void Flashcard::writeChildData(std::wofstream& stream)
{
	if (_caseSensitive)
		Option(Globals::optionCaseSensitive).write(stream);
	stream << _question << L"\n";
	stream << _answer << L"\n";
}

bool Flashcard::isCaseSensitive()
{
	return _caseSensitive;
}

void Flashcard::setCaseSensitive()
{
	_caseSensitive = true;
}

void Flashcard::setCaseInsensitive()
{
	_caseSensitive = false;
}
