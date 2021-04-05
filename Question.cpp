#include "Question.h"
#include "Flashcard.h"

std::vector<Question&> Question::_questionList = std::vector<Question&>();

Question::Question(std::vector<std::wstring> tags)
{
	_tags = tags;
}

std::vector<Question&> Question::getQuestionList()
{
	return _questionList;
}

void Question::readQuestionList()
{
	_questionList = std::vector<Question&>();
	_questionList += Flashcard::readFlashcardList();
}

void Question::appendQuestionToList(Question& question)
{
	_questionList.push_back(question);
}

void Question::appendQuestionsToList(std::vector<Question&> questions)
{
	_questionList.insert(_questionList.end(), questions.begin(), questions.end());
}

std::vector<std::wstring> Question::getTags()
{
	return _tags;
}
