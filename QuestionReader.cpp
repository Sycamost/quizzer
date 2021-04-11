#include "QuestionReader.h"
#include "Option.h"
#include "util.h"
#include "globals.h"

QuestionReader::QuestionReader(
	void(*readChildData)(std::wstring line),
	Question*(*constructCurrent)(std::vector<Option> options, std::vector<std::wstring> tags))
	:
	_readChildData(readChildData),
	_constructCurrent(constructCurrent)
{}

Question* QuestionReader::read(std::wifstream& stream)
{
	std::vector<Option> options = Option::readOptions(stream);
	Stage stage = Stage::CHILD_DATA;
	std::vector<std::wstring> tags = std::vector<std::wstring>();

	while (!stream.eof())
	{
		std::wstring line = getInputLine(stream);

		if (line == L"")
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
}
