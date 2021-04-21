#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include "Handlers.h"

#define DEFINE_CMD_FUNC(func) const CommandFunc func
#define DECLARE_CMD_FUNC(func) const CommandFunc func = [](easy_list::list<std::wstring> args = easy_list::list<std::wstring>()) -> InputHandler::Returns

enum class CommandType {
	CANCEL,
	QUIT,
	EXIT,
	WRITE,
	BOOST,
	CONCEDE,
	PLAY
};

typedef InputHandler::Returns (*CommandFunc) (easy_list::list<std::wstring> args);

class CommandInfo {
public:
	const CommandType getType() const { return _type; }
	const std::wstring getCode() const { return _code; }
	const CommandFunc* getFunc() const { return _func; }
	const InputHandler::Returns callFunc(easy_list::list<std::wstring> args) { return (*_func)(args); }
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
	InputHandler::Returns doCommandFunc();
	CommandInfo getCommandInfo();
	const easy_list::list<std::wstring> getArgs() const;
private:
	CommandInfo _commandInfo;
	easy_list::list<std::wstring> _args;
	Command(easy_list::list<std::wstring> args, CommandInfo commandInfo);
};