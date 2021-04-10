#include "Question.h"
#include "Flashcard.h"
#include "util.h"
#include "globals.h"

std::vector<Question*> Question::_questionList = std::vector<Question*>();

extern const std::vector<QuestionTypeInfo> questionTypeInfos = std::vector<QuestionTypeInfo>({
	QuestionTypeInfo(QuestionType::FLASHCARD, L"flashcard", L"FLASHCARD", "flashcards.txt")
});

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

	try
	{
		std::wifstream file(Globals::flashcardsFileAddress);
		if (file.is_open())
		{
			while (!file.eof())
			{

			}
		}
	}
	catch (std::exception e)
	{
		std::wcout << L"Error reading flashcards from file.\n";
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

const std::wstring questionTypeCode(const QuestionType qt)
{
	auto iter = std::find_if(
		questionTypeInfos.begin(),
		questionTypeInfos.end(),
		[qt](QuestionTypeInfo qti) -> bool { return qti.type == qt; });
	if (iter == questionTypeInfos.end())
		return L"";
	return iter->code;
}

const QuestionTypeInfo* getQuestionTypeInfo(QuestionType type)
{
	auto iter = std::find_if(
		questionTypeInfos.begin(),
		questionTypeInfos.end(),
		[type](QuestionTypeInfo qti) -> bool { return qti.type == type; });
	if (iter == questionTypeInfos.end())
		return nullptr;
	return iter._Ptr;
}

const QuestionTypeInfo* getQuestionTypeInfoFromCode(std::wstring code)
{
	auto iter = std::find_if(
		questionTypeInfos.begin(),
		questionTypeInfos.end(),
		[code](QuestionTypeInfo qti) -> bool { return qti.code == code; });
	if (iter == questionTypeInfos.end())
		return nullptr;
	return iter._Ptr;
}
