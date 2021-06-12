#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include <template_helpers.h>
#include "HandlerReturns.h"

#define DECLARE_CMD_FUNC(funcName) CommandHandlerReturns funcName(easy_list::list<std::wstring> args, CommandFlagCollection flags)
#define DEFINE_CMD_FUNC(funcName) CommandHandlerReturns funcName(easy_list::list<std::wstring> args, CommandFlagCollection flags)

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

/// <summary>
/// Info associated with a type of flag (i.e. abstracted from any user-given values it might have when used).
/// </summary>
class CommandFlagInfo {
public:
	const std::wstring getFirstCode() const { return _codes[0]; }
	const bool hasCode(const std::wstring code) const { return _codes.contains(code); }
	const CommandType getCommandType() const { return _commandType; }
	static const easy_list::list<CommandFlagInfo>* getList();
	static const easy_list::list<std::pair<CommandFlagInfo, CommandFlagInfo>>* getContradictoriesList();
	bool operator==(const CommandFlagInfo& other) const;
private:
	const CommandType _commandType;
	const easy_list::list<std::wstring> _codes;
	const bool _requiresValue;
	const bool _allowsValue;
	CommandFlagInfo(CommandType commandType, easy_list::list<std::wstring> codes) :
		_commandType(commandType),
		_codes(codes),
		_requiresValue(false),
		_allowsValue(false)
	{}
	CommandFlagInfo(CommandType commandType, easy_list::list<std::wstring> codes, bool takesValue) :
		_commandType(commandType),
		_codes(codes),
		_requiresValue(takesValue),
		_allowsValue(takesValue)
	{}
	CommandFlagInfo(CommandType commandType, easy_list::list<std::wstring> codes, bool requiresValue, bool allowsValue) :
		_commandType(commandType),
		_codes(codes),
		_requiresValue(requiresValue),
		_allowsValue(allowsValue)
	{}
};

/// <summary>
/// A concrete instance of a flag, i.e. flag type together with any value it has.
/// </summary>
class CommandFlag
{
public:
	CommandFlag(CommandFlagInfo info) :
		_info(info),
		_hasValue(false),
		_value(L"")
	{}
	CommandFlag(CommandFlagInfo info, std::wstring value) :
		_info(info),
		_hasValue(true),
		_value(value)
	{}
	bool isValid(CommandType commandType) const;
	bool contradicts(CommandFlag other) const;
	bool operator==(const CommandFlag& rhs) const;
	std::wstring toString() const {
		if (isValid())
			return _info.getFirstCode() + L"=" + _value;
		return _info.getFirstCode();
	}
private:
	CommandFlagInfo _info;
	bool _hasValue;
	std::wstring _value;
};

/// <summary>
/// A collection of concrete instances of flags.
/// </summary>
/// <seealso cref="CommandFlag"/>
class CommandFlagCollection
{
public:
	CommandFlagCollection();
	CommandFlagCollection(const CommandFlag& flag);
	template<typename List, std::enable_if_t<std::is_convertible_v<List, easy_list::list<CommandFlag>>, bool> = true>
	CommandFlagCollection(const List& data)
	{
		_data = static_cast<easy_list::list<CommandFlag>>(data);
	}
	const easy_list::list<CommandFlag> getList() const { return _data; }
	bool isValid(CommandType commandType) const;
	CommandFlagCollection& add(const CommandFlag& rhs);
	CommandFlagCollection& add(const CommandFlagCollection& rhs);
	bool sharesAny(const CommandFlagCollection& other) const;
	std::wstring toString() const { return _data.transform<std::wstring>([](CommandFlag flag) -> std::wstring { return flag.toString(); }); }
private:
	easy_list::list<CommandFlag> _data;
};


using CommandFunc = CommandHandlerReturns(*)(easy_list::list<std::wstring> args, CommandFlagCollection flags);

class CommandInfo {
public:
	const CommandType getType() const { return _type; }
	const std::wstring getFirstCode() const { return _codes[0]; }
	const bool hasCode(const std::wstring code) const { return _codes.contains(code); }
	const int countValidFlags(CommandFlagCollection flags);
	const CommandFunc getFunc() const { return _func; }
	DEFINE_CMD_FUNC(callFunc) const { return _func(args, flags); }
	static const easy_list::list<CommandInfo>* getList();
	static const std::wstring getFirstCode(const CommandType ct);
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