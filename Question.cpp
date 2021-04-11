#include "Question.h"
#include "Flashcard.h"
#include "util.h"
#include "globals.h"
#include "QuestionTypeInfo.h"

std::vector<Question*> Question::_questionList = std::vector<Question*>();

std::vector<std::wstring> Question::readTags(std::wifstream& stream)
{
	std::vector<std::wstring> tags = std::vector<std::wstring>();
	while (!stream.eof())
	{
		std::wstring line = getInputLine(stream);
		if (line == L"")
			break;
		if (line == L"%tags")
			continue;
		tags.push_back(line);
	}
	return tags;
}

Question::Question(QuestionType type, std::vector<std::wstring> tags) :
	_type(type),
	_tags(tags)
{}

void Question::write(std::wofstream& stream)
{
	writeChildData(stream);
	stream << Globals::fileEscapeChar << L"tags\n";
	for (unsigned int i = 0; i < _tags.size(); i++)
		stream << _tags[i] << L"\n";
	stream << L"\n";
}

std::vector<Question*> Question::getQuestionList()
{
	return _questionList;
}

void Question::readQuestionList()
{
	_questionList = std::vector<Question*>();
	std::wifstream file;

	for 
	try
	{
		file.open(Globals::flashcardsFileAddress);
		if (!file.is_open())
			throw new std::exception("File didn't open correctly.");
	}
	catch (std::exception e)
	{
		std::wcout << L"Error reading flashcards from file.\n";
		return;
	}
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

QuestionType Question::getType()
{
	return _type;
}
