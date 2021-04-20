#include "Question.h"
#include "Flashcard.h"
#include "util.h"
#include "globals.h"
#include "QuestionTypeInfo.h"

easy_list::list<std::wstring> Question::readTags(std::wifstream& stream)
{
	easy_list::list<std::wstring> tags = easy_list::list<std::wstring>();
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

Question::Question(QuestionType type, easy_list::list<std::wstring> tags) :
	_type(type),
	_tags(tags)
{}

void Question::write(std::wofstream& stream)
{
	writeChildData(stream);
	stream << Globals::fileEscapeChar << L"tags\n";
	for (auto tag : _tags)
		stream << tag << L"\n";
	stream << L"\n";
}

easy_list::list<std::wstring> Question::getTags()
{
	return _tags;
}

QuestionType Question::getType()
{
	return _type;
}
