#pragma once
#include <string>
#include <vector>
#include <fstream>

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
	std::wstring getArgument(int n) const;
	std::vector<std::wstring> getArguments() const;
	static std::vector<Option> readOptions(std::wifstream& stream);
	void write(std::wofstream& stream);
};