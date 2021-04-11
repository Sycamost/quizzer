#include "QuestionReader.h"
#include "Option.h"
#include "util.h"
#include "globals.h"

QuestionReader::QuestionReader(
	QuestionType type,
	void(*readChildData)(std::wstring line),
	void(*constructCurrent)(std::vector<std::wstring> tags))
	:
	_type(type),
	_stage(Stage::CHILD_DATA),
	_tags(std::vector<std::wstring>()),
	_readChildData(readChildData),
	_constructCurrent(constructCurrent)
{}

Question* QuestionReader::read(std::wifstream& stream)
{
	std::vector<Option> options = Option::readOptions(stream);
	setStage(Stage::CHILD_DATA);

	while (!stream.eof())
	{
		std::wstring line = getInputLine(stream);

		if (line == L"")
		{
			return _constructCurrent(_tags);
		}

		else if (line == Globals::fileStartOfTags)
		{
			setStage(Stage::TAGS);
			continue;
		}

		if (_stage == Stage::CHILD_DATA)
		{
			_readChildData(line);
			continue;
		}

		else if (_stage == Stage::TAGS)
		{
			_tags.push_back(line);
			continue;
		}
	}
}
