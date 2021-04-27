#include <time.h>
#include "MultipleChoice.h"
#include "QuestionType.h"
#include "util.h"

MultipleChoice::MultipleChoice(std::wstring question, std::wstring correctAnswer, easy_list::list<std::wstring> wrongAnswers, easy_list::list<std::wstring> tags) :
	Question(QuestionType::MULTIPLE_CHOICE, tags),
	_question(question),
	_answers(wrongAnswers + correctAnswer),
	_correctAnswerIndex(wrongAnswers.size())
{}

std::wstring optionToken(const size_t index)
{
	// This will only work if there are at most 26 options - should be ok...
	return std::wstring(1, L'a' + (wchar_t)index);
}

std::wstring MultipleChoice::getQuestion()
{
	shuffle();
	std::wstring message = _question + L"\n";
	for (size_t i = 0; i < _answers.size(); i++)
	{
		message += L"\t(" + optionToken(i) + L") ";
		message += _answers[i];
		message += L"\n";
	}
	message += L"Answer simply as a letter, e.g. \"a\".";
	return message;
}

std::wstring MultipleChoice::getAnswer()
{
	return _answers[_correctAnswerIndex];
}

bool MultipleChoice::isCorrect(std::wstring guess)
{
	guess = toLower(guess);

	// If "x" is the correct option token, accept -
	// *  x
	// *  x)
	// * (x)

	auto correctOption = optionToken(_correctAnswerIndex);

	if (guess == correctOption ||
		guess == L"(" + correctOption + L")" ||
		guess == correctOption + L")")
	{
		return true;
	}

	return false;
}

void MultipleChoice::writeChildData(std::wofstream& stream)
{
	stream << _question << L"\n";
	stream << _answers[_correctAnswerIndex] << L"\n";
	for (auto answer : _answers.removeAt(_correctAnswerIndex))
		stream << answer << L"\n";
}

void MultipleChoice::shuffle()
{
	std::wstring correctAnswer = _answers[_correctAnswerIndex];
	_answers = _answers.removeFirst(correctAnswer).shuffle();
	std::srand(_correctAnswerIndex * static_cast<unsigned int>(correctAnswer[0]) * std::time(NULL));
	_correctAnswerIndex = std::rand() % (_answers.size() + 1);
	_answers.insert(_answers.begin() + _correctAnswerIndex - 1, correctAnswer);
}