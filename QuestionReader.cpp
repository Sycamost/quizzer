#include <easy_list.h>
#include "QuestionReader.h"
#include "Option.h"
#include "util.h"
#include "globals.h"

QuestionReader::QuestionReader(
	void(*clearChildData)(),
	void(*readChildData)(std::wstring line),
	Question*(*constructCurrent)(easy_list::list<Option> options, easy_list::list<std::wstring> tags))
	:
	_clearChildData(clearChildData),
	_readChildData(readChildData),
	_constructCurrent(constructCurrent)
{}

Question* QuestionReader::read(std::wifstream& stream)
{
	Stage stage = Stage::CHILD_DATA;
	easy_list::list<std::wstring> tags = easy_list::list<std::wstring>();
	_clearChildData();

	easy_list::list<Option> options = Option::readOptions(stream);

	while (!stream.eof())
	{
		std::wstring line = getInputLine(stream);

		if (line == L"" || stream.eof())
		{
			return _constructCurrent(options, tags);
		}

		else if (line == Globals::fileStartOfTags)
		{
			stage = Stage::TAGS;
			continue;
		}

		if (stage == Stage::CHILD_DATA)
		{
			_readChildData(line);
			continue;
		}

		else if (stage == Stage::TAGS)
		{
			tags.push_back(line);
			continue;
		}
	}

	if (stage == Stage::TAGS)
		return _constructCurrent(options, tags);
	else
		return nullptr;
}
