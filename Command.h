#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "CmdHandler.h"

class Command
{
public:
	static Command* read(std::wifstream& stream);
	CmdHandler::Returns doCmdFunc();
	typedef CmdHandler::Returns (*CmdFunc) (std::vector<std::wstring> args);
	static const std::wstring cmdWriteOptFlashcard;
private:
	std::wstring _cmd;
	std::vector<std::wstring> _args;
	CmdFunc _cmdFunc;
	static const std::map<std::wstring, CmdFunc> _cmdFuncs;
	Command(std::wstring cmd, std::vector<std::wstring> args, CmdFunc cmdFunc);
};