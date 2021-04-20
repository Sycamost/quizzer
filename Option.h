#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>

class Option
{
protected:
	std::wstring _option;
	easy_list::list<std::wstring> _args;
	void constructor(std::wstring option, easy_list::list<std::wstring> args);
public:
	Option(std::wstring option);
	Option(std::wstring option, std::wstring arg);
	Option(std::wstring option, easy_list::list<std::wstring> args);;
	std::wstring getOption() const;
	std::wstring getArgument(unsigned int n) const;
	easy_list::list<std::wstring> getArguments() const;
	static easy_list::list<Option> readOptions(std::wifstream& stream);
	static Option* readOption(std::wstring userInput);
	void write(std::wofstream& stream);
};