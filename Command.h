#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include "HandlerReturns.h"

#define DECLARE_CMD_FUNC(funcName) CommandHandlerReturns funcName(easy_list::list<std::wstring> args = easy_list::list<std::wstring>())
#define DEFINE_CMD_FUNC(funcName) CommandHandlerReturns funcName(easy_list::list<std::wstring> args)

enum class CommandType {
	CANCEL,
	QUIT_MAIN,
	QUIT_PLAY,
	QUIT_WRITE,
	WRITE,
	BOOST,
	CONCEDE,
	PLAY
};

using CommandFunc = CommandHandlerReturns(*)(easy_list::list<std::wstring> args);

class CommandInfo {
public:
	const CommandType getType() const { return _type; }
	const std::wstring getCode() const { return _codes[0]; }
	const bool hasCode(const std::wstring code) const { return _codes.contains(code); }
	const CommandFunc getFunc() const { return _func; }
	CommandHandlerReturns callFunc(easy_list::list<std::wstring> args) const { return _func(args); }
	static const easy_list::list<CommandInfo>* getList();
	static const std::wstring getFirstCode(const CommandType ct);
	bool operator==(const CommandInfo& other)
	{
		return _type == other._type;
	}
private:
	const CommandType _type;
	const easy_list::list<std::wstring> _codes;
	const CommandFunc _func;
	CommandInfo(const CommandType type, easy_list::list<std::wstring> codes, const CommandFunc func) :
		_type(type),
		_codes(codes),
		_func(func)
	{}
};


class Command
{
public:
	static easy_list::list<Command> makePossibleCommands(std::wstring input);
	CommandHandlerReturns doCommandFunc() const;
	const CommandInfo getCommandInfo() const;
	const easy_list::list<std::wstring> getArgs() const;
private:
	CommandInfo _commandInfo;
	easy_list::list<std::wstring> _args;
	Command(easy_list::list<std::wstring> args, CommandInfo commandInfo);
};