#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <easy_list.h>

class Option
{
protected:
	std::wstring _option;
	std::vector<std::wstring> _args;
	void constructor(std::wstring option, std::vector<std::wstring> args);
public:
	Option(std::wstring option);
	Option(std::wstring option, std::wstring arg);
	Option(std::wstring option, std::vector<std::wstring> args);;
	std::wstring getOption() const;
	std::wstring getArgument(unsigned int n) const;
	std::vector<std::wstring> getArguments() const;
	static easy_list::list<Option> readOptions(std::wifstream& stream);
	static Option* readOption(std::wstring userInput);
	void write(std::wofstream& stream);
};