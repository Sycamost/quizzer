#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <easy_list.h>
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
	const CommandType getType() const { return _type; }
	const std::wstring getCode() const { return _code; }
	const CommandFunc* getFunc() const { return _func; }
	const InputHandler::Returns callFunc(std::vector<std::wstring> args) { return (*_func)(args); }
	static const easy_list::list<CommandInfo>* getList();
	static const easy_list::list<CommandInfo>::const_iterator get(const CommandType type);
	static const easy_list::list<CommandInfo>::const_iterator get(const std::wstring code);
private:
	const CommandType _type;
	const std::wstring _code;
	const CommandFunc* _func;
	CommandInfo(const CommandType type, const std::wstring code, const CommandFunc* func) :
		_type(type),
		_code(code),
		_func(func)
	{}
};


class Command
{
public:
	static Command* read(std::wifstream& stream);
	static Command* read(std::wstring userInput);
	InputHandler::Returns doCommandFunc();
	CommandInfo getCommandInfo();
	const std::vector<std::wstring> getArgs() const;
private:
	CommandInfo _commandInfo;
	std::vector<std::wstring> _args;
	Command(std::vector<std::wstring> args, CommandInfo commandInfo);
};