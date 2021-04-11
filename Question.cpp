#include "Question.h"
#include "Flashcard.h"
#include "util.h"
#include "globals.h"
#include "QuestionTypeInfo.h"

std::vector<std::wstring> Question::readTags(std::wifstream& stream)
{
	std::vector<std::wstring> tags = std::vector<std::wstring>();
	while (!stream.eof())
	{
		std::wstring line = getInputLine(stream);
		if (line == L"")
			break;
		if (line == Globals::fileEscapeChar + L"tags")
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

std::vector<std::wstring> Question::getTags()
{
	return _tags;
}

QuestionType Question::getType()
{
	return _type;
}
