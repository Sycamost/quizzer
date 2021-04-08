#include <string>
#include <vector>
#include "Option.h"
#include "util.h"
#include "globals.h"

std::vector<Option> Option::readOptions(std::wifstream& stream)
{
	std::vector<Option> options = std::vector<Option>();
	while (!stream.eof())
	{
		std::streampos pos = stream.tellg();
		std::vector<std::wstring> lineWords = splitByWord(getInputLine(stream));
		if (lineWords.size() == 0)
			break;

		if (lineWords[0].substr(0, Globals::fileEscapeChar.size()) != Globals::fileEscapeChar)
		{
			stream.seekg(pos);
			break;
		}

		options.push_back(Option(
			lineWords[0].substr(Globals::fileEscapeChar.size()),
			std::vector<std::wstring>(lineWords.begin() + 1, lineWords.end())));
	}
	return options;
}

void Option::constructor(std::wstring option, std::vector<std::wstring> args)
{
	_option = option;
	_args = args;
}

Option::Option(std::wstring option)
{
	constructor(option, std::vector<std::wstring>());
}

Option::Option(std::wstring option, std::wstring arg)
{
	constructor(option, std::vector<std::wstring>(1, arg));
}

Option::Option(std::wstring option, std::vector<std::wstring> args)
{
	constructor(option, args);
}

std::wstring Option::getOption() const
{
	return _option;
}

std::wstring Option::getArgument(unsigned int n) const
{
	if (n >= _args.size())
		return L"";
	return _args[n];
}

std::vector<std::wstring> Option::getArguments() const
{
	return _args;
}

void Option::write(std::wofstream& stream)
{
	stream << Globals::fileEscapeChar << _option;
	for (unsigned int i = 0; i < _args.size(); i++)
		stream << L" " << _args[i];
	stream << L"\n";
}
