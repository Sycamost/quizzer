#include "Question.h"
#include "Flashcard.h"
#include "util.h"

std::vector<Question*> Question::_questionList = std::vector<Question*>();

void Question::writeTags(std::wofstream& stream)
{
	stream << L"%tags\n";
	for (unsigned int i = 0; i < _tags.size(); i++)
		stream << _tags[i] << L"\n";
}

std::vector<std::wstring> Question::readTags(std::wifstream& stream)
{
	std::vector<std::wstring> tags = std::vector<std::wstring>();
	while (!stream.eof())
	{
		std::wstring line = getUserInputLine(stream);
		if (line == L"")
			break;
		if (line == L"%tags")
			continue;
		tags.push_back(line);
	}
	return tags;
}

Question::Question(std::vector<std::wstring> tags)
{
	_tags = tags;
}

std::vector<Question*> Question::getQuestionList()
{
	return _questionList;
}

void Question::readQuestionList()
{
	_questionList = std::vector<Question*>();
	appendQuestionsToList(Flashcard::readFlashcardList());
}

void Question::appendQuestionToList(Question* question)
{
	_questionList.push_back(question);
}

void Question::appendQuestionsToList(std::vector<Question*> questions)
{
	_questionList.insert(_questionList.end(), questions.begin(), questions.end());
}

std::vector<std::wstring> Question::getTags()
{
	return _tags;
}
