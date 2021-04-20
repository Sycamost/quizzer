#include <string>
#include <easy_list.h>
#include "Option.h"
#include "util.h"
#include "globals.h"

easy_list::list<Option> Option::readOptions(std::wifstream& stream)
{
	auto options = easy_list::list<Option>();
	while (!stream.eof())
	{
		std::streampos pos = stream.tellg();
		Option* option = readOption(getInputLine(stream));
		if (option == nullptr)
		{
			stream.seekg(pos);
			break;
		}
		options.push_back(*option);
	}
	return options;
}

Option* Option::readOption(std::wstring userInput)
{
	easy_list::list<std::wstring> words = splitByWord(toUpper(userInput));
	if (words.size() == 0)
		return nullptr;

	// Starts with file escape char?
	if (words[0].substr(0, Globals::fileEscapeChar.size()) != Globals::fileEscapeChar)
		return nullptr;

	return new Option(
		words[0].substr(Globals::fileEscapeChar.size()),
		easy_list::list<std::wstring>(words.begin() + 1, words.end()));
}

void Option::constructor(std::wstring option, easy_list::list<std::wstring> args)
{
	_option = option;
	_args = args;
}

Option::Option(std::wstring option)
{
	constructor(option, easy_list::list<std::wstring>());
}

Option::Option(std::wstring option, std::wstring arg)
{
	constructor(option, easy_list::list<std::wstring>(1, arg));
}

Option::Option(std::wstring option, easy_list::list<std::wstring> args)
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

easy_list::list<std::wstring> Option::getArguments() const
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
