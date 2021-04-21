#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include "HandlerReturns.h"

#define DECLARE_CMD_FUNC(funcName) const CommandFunc funcName
#define DEFINE_CMD_FUNC(funcName) const CommandFunc funcName = [](easy_list::list<std::wstring> args = easy_list::list<std::wstring>()) -> CommandHandlerReturns

enum class CommandType {
	CANCEL,
	QUIT,
	EXIT,
	WRITE,
	BOOST,
	CONCEDE,
	PLAY
};

using CommandFunc = CommandHandlerReturns(*)(easy_list::list<std::wstring> args);

class CommandInfo {
public:
	const CommandType getType() const { return _type; }
	const std::wstring getCode() const { return _code; }
	const CommandFunc* getFunc() const { return _func; }
	CommandHandlerReturns callFunc(easy_list::list<std::wstring> args) const { return (*_func)(args); }
	static const easy_list::list<CommandInfo>* getList();
	static const easy_list::list<CommandInfo>::const_iterator get(const CommandType type);
	static const easy_list::list<CommandInfo>::const_iterator get(const std::wstring code);
	bool operator==(const CommandInfo& other)
	{
		return _type == other._type;
	}
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
	CommandHandlerReturns doCommandFunc() const;
	const CommandInfo getCommandInfo() const;
	const easy_list::list<std::wstring> getArgs() const;
private:
	CommandInfo _commandInfo;
	easy_list::list<std::wstring> _args;
	Command(easy_list::list<std::wstring> args, CommandInfo commandInfo);
};