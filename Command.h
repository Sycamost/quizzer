#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include <template_helpers.h>
#include "HandlerReturns.h"

#define DECLARE_CMD_FUNC(funcName) CommandHandlerReturns funcName(easy_list::list<std::wstring> args, CommandFlags flags)
#define DEFINE_CMD_FUNC(funcName) CommandHandlerReturns funcName(easy_list::list<std::wstring> args, CommandFlags flags)

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

enum class CommandFlags : char {
	NONE = 0,
	//FIRST = 1 << 0,
	//SECOND = 1 << 1,
	//THIRD = 1 << 2,
	//FOURTH = 1 << 3,
};

inline CommandFlags operator&(CommandFlags lhs, CommandFlags rhs)
{
	return (CommandFlags)((char)lhs & (char)rhs);
}

inline CommandFlags operator|(CommandFlags lhs, CommandFlags rhs)
{
	return (CommandFlags)((char)lhs | (char)rhs);
}

inline CommandFlags operator~(CommandFlags flags)
{
	return (CommandFlags)~(char)flags;
}

inline CommandFlags operator&=(CommandFlags lhs, CommandFlags rhs)
{
	return lhs = (CommandFlags)((char)lhs & (char)rhs);
}

inline CommandFlags operator|=(CommandFlags lhs, CommandFlags rhs)
{
	return lhs = (CommandFlags)((char)lhs | (char)rhs);
}

template<typename ..._CommandFlags, std::enable_if_t<std::conjunction_v<std::is_same<_CommandFlags, CommandFlags>...>, bool> = true>
inline bool sharesAny(CommandFlags flags, CommandFlags first, _CommandFlags... rest)
{
	if ((flags & first) != CommandFlags::NONE) return true;
	for each (CommandFlags other in std::tuple<CommandFlags>(rest...))
	{
		if ((flags & other) != CommandFlags::NONE)
			return true;
	}
	return false;
}

class CommandFlagInfo {
public:
	const CommandFlags getFlag() const { return _flag; }
	const std::wstring getFirstCode() const { return _codes[0]; }
	const bool hasCode(const std::wstring code) const { return _codes.contains(code); }
	static const easy_list::list<CommandFlagInfo>* getList();
	static const std::wstring getFirstCode(const CommandFlags flag);
	bool operator==(const CommandFlagInfo& other)
	{
		return _flag == other._flag;
	}
private:
	const CommandFlags _flag;
	const easy_list::list<std::wstring> _codes;
	CommandFlagInfo(const CommandFlags flag, easy_list::list<std::wstring> codes) :
		_flag(flag),
		_codes(codes)
	{}
};

using CommandFunc = CommandHandlerReturns(*)(easy_list::list<std::wstring> args, CommandFlags flags);

class CommandInfo {
public:
	const CommandType getType() const { return _type; }
	const std::wstring getFirstCode() const { return _codes[0]; }
	const bool hasCode(const std::wstring code) const { return _codes.contains(code); }
	const CommandFunc getFunc() const { return _func; }
	DEFINE_CMD_FUNC(callFunc) const { return _func(args, flags); }
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
	const CommandFlags getFlags() const;
private:
	CommandInfo _commandInfo;
	easy_list::list<std::wstring> _args;
	CommandFlags _flags;
	Command(easy_list::list<std::wstring> args, CommandFlags flags, CommandInfo commandInfo);
};