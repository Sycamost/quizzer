#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <utility>
#include "CmdHandler.h"

#define DEFINE_CMD_FUNC(func) const CommandFunc func
#define DECLARE_CMD_FUNC(func) const CommandFunc func = [](std::vector<std::wstring> args) -> CmdHandler::Returns

enum class CommandType {
	CANCEL,
	QUIT,
	EXIT,
	WRITE,
	CASE,
	BOOST,
	FINISH,
	PLAY
};

typedef CmdHandler::Returns (*CommandFunc) (std::vector<std::wstring> args);

struct CommandInfo {
	CommandType type;
	CommandFunc func;
	CommandInfo() {
		throw new std::exception("CommandInfo not initialized properly.");
	}
	CommandInfo(CommandType ty, CommandFunc fn) {
		type = ty;
		func = fn;
	}
};

class Command
{
public:
	static Command* read(std::wifstream& stream);
	static Command* read(std::wstring userInput);
	CmdHandler::Returns doCommandFunc();
	CommandInfo getCommandInfo();
	static const std::wstring cmdWriteOptFlashcard;
private:
	CommandInfo _commandInfo;
	std::wstring _code;
	std::vector<std::wstring> _args;
	static const std::map<std::wstring, CommandInfo> _commandInfos;
	Command(std::wstring code, std::vector<std::wstring> args, CommandInfo commandInfo);
};