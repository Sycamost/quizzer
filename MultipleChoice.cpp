#include "MultipleChoice.h"
#include "QuestionType.h"
#include "util.h"

MultipleChoice::MultipleChoice(std::wstring question, std::wstring correctAnswer, easy_list::list<std::wstring> wrongAnswers, easy_list::list<std::wstring> tags) :
	Question(QuestionType::MULTIPLE_CHOICE, tags),
	_question(question),
	_correctAnswer(correctAnswer),
	_wrongAnswers(wrongAnswers),
	_shuffledIndexes({})
{
	// -1 represents "correct answer"
	_shuffledIndexes.push_back(-1);
	// 0, 1, ... represent index of the list of wrong answers
	for (size_t i = 0; i < _wrongAnswers.size(); i++)
		_shuffledIndexes.push_back(i);
}

std::wstring optionToken(const size_t index)
{
	// This will only work if there are at most 26 options - should be ok...
	return std::wstring(1, L'a' + (wchar_t)index);
}

std::wstring MultipleChoice::getQuestion()
{
	shuffle();
	std::wstring message = _question + L"\n";
	for (size_t i = 0; i < _wrongAnswers.size() + 1; i++)
	{
		message += L"\t(" + optionToken(i) + L") ";
		message += getOption(i);
		message += L"\n";
	}
	message += L"Answer simply as a letter, e.g. \"a\".";
	return message;
}

std::wstring MultipleChoice::getAnswer()
{
	return _correctAnswer;
}

bool MultipleChoice::isCorrect(std::wstring guess)
{
	guess = toLower(guess);

	// If "x" is the correct option token, accept -
	// *  x
	// *  x)
	// * (x)

	std::wstring correctOption = optionToken(std::distance(_shuffledIndexes.cbegin(), _shuffledIndexes.search(-1)));

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
	stream << _correctAnswer << L"\n";
	for (auto answer : _wrongAnswers)
		stream << answer << L"\n";
}

void MultipleChoice::shuffle()
{
	_shuffledIndexes.shuffle();
}

std::wstring MultipleChoice::getOption(size_t unshuffledIndex)
{
	size_t index = _shuffledIndexes[unshuffledIndex];
	if (index == (size_t)-1)
		return _correctAnswer;
	return _wrongAnswers[index];
}
