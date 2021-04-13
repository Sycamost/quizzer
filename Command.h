#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "InputHandler.h"

#define DEFINE_CMD_FUNC(func) const CommandFunc func
#define DECLARE_CMD_FUNC(func) const CommandFunc func = [](std::vector<std::wstring> args = std::vector<std::wstring>()) -> InputHandler::Returns

enum class CommandType {
	CANCEL,
	QUIT,
	EXIT,
	WRITE,
	BOOST,
	CONCEDE,
	PLAY
};

typedef InputHandler::Returns (*CommandFunc) (std::vector<std::wstring> args);

class CommandInfo {
public:
	const CommandType type;
	const std::wstring code;
	const CommandFunc* func;
	CommandInfo(const CommandType ty, const std::wstring c, const CommandFunc* fn) :
		type(ty),
		code(c),
		func(fn)
	{}
	bool isType(CommandType ty)
	{
		return type == ty;
	}
	bool isCode(std::wstring wstr)
	{
		return code == wstr;
	}
};

class Command
{
public:
	static Command* read(std::wifstream& stream);
	static Command* read(std::wstring userInput);
	static const CommandInfo* getCommandInfo(CommandType type);
	static const CommandInfo* getCommandInfo(std::wstring code);
	InputHandler::Returns doCommandFunc();
	CommandInfo getCommandInfo();
	const std::vector<std::wstring> getArgs() const;
private:
	CommandInfo _commandInfo;
	std::vector<std::wstring> _args;
	static const std::vector<CommandInfo> _commandInfos;
	Command(std::vector<std::wstring> args, CommandInfo commandInfo);
};