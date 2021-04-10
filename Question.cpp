#include "Question.h"
#include "Flashcard.h"
#include "util.h"

std::vector<Question*> Question::_questionList = std::vector<Question*>();

typedef std::map<QuestionType, std::wstring> MapQtWstr;
typedef std::pair<QuestionType, std::wstring> PairQtWstr;
const MapQtWstr questionTypeDisplay = MapQtWstr({
	PairQtWstr(QuestionType::FLASHCARD, L"flashcard")
});
typedef std::map<std::wstring, QuestionType> MapWstrQt;
typedef std::pair<std::wstring, QuestionType> PairWstrQt;
const MapWstrQt questionCodeType = MapWstrQt({
	PairWstrQt(L"FLASHCARD", QuestionType::FLASHCARD)
});

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
{
	tags.size();
	_tags.size();
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

QuestionType Question::getType()
{
	return _type;
}

const std::wstring questionTypeCode(const QuestionType qt)
{
	auto iter = std::find_if(
		questionCodeType.begin(),
		questionCodeType.end(),
		[qt](std::pair<std::wstring, QuestionType> pair) -> bool { return pair.second == qt; });
	if (iter == questionCodeType.end())
		return L"";
	return iter->first;
}
