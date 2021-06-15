#pragma once
#include "CommandType.h"
#include <easy_list.h>

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
	static CommandFlag* read(std::wstring input);
	bool isValid(CommandType commandType) const;
	bool contradicts(CommandFlag other) const;
	bool operator==(const CommandFlag& rhs) const;
	std::wstring toString() const {
		if (_hasValue)
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