#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include <template_helpers.h>
#include "HandlerReturns.h"
#include "CommandFlag.h"

#define DECLARE_CMD_FUNC(funcName) CommandHandlerReturns funcName(easy_list::list<std::wstring> args, CommandFlagCollection flags)
#define DEFINE_CMD_FUNC(funcName) CommandHandlerReturns funcName(easy_list::list<std::wstring> args, CommandFlagCollection flags)


using CommandFunc = CommandHandlerReturns(*)(easy_list::list<std::wstring> args, CommandFlagCollection flags);

class CommandInfo {
public:
	const CommandType getType() const { return _type; }
	const std::wstring getFirstCode() const { return _codes[0]; }
	const bool hasCode(const std::wstring code) const { return _codes.contains(code); }
	int countValidFlags(CommandFlagCollection flags);
	const CommandFunc getFunc() const { return _func; }
	DEFINE_CMD_FUNC(callFunc) const { return _func(args, flags); }
	static const easy_list::list<CommandInfo>* getList();
	static const std::wstring getFirstCode(const CommandType ct);
	static DECLARE_CMD_FUNC(cmdFuncHelp);
	bool operator==(const CommandInfo& other);
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
	const CommandFlagCollection getFlags() const;
private:
	CommandInfo _commandInfo;
	easy_list::list<std::wstring> _args;
	CommandFlagCollection _flags;
	Command(easy_list::list<std::wstring> args, CommandFlagCollection flags, CommandInfo commandInfo);
};