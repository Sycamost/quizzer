#include "CommandFlag.h"

bool CommandFlagInfo::operator==(const CommandFlagInfo& other) const
{
	return (const CommandFlagInfo*)this == &other;
}

const easy_list::list<CommandFlagInfo>* CommandFlagInfo::getList()
{
	static auto list = easy_list::list<CommandFlagInfo>({
		// CommandFlagInfo declarations go here
		});
	return &list;
}

const easy_list::list<std::pair<CommandFlagInfo, CommandFlagInfo>>* CommandFlagInfo::getContradictoriesList()
{
	static auto list = easy_list::list<std::pair<CommandFlagInfo, CommandFlagInfo>>({
		// Lists of contradictory CommandFlagInfos go here
		});
	return &list;
}

bool CommandFlag::isValid(CommandType commandType) const
{
	return (!_hasValue || !_value.empty()) && commandType == _info.getCommandType();
}

bool CommandFlag::contradicts(CommandFlag other) const
{
	if (_info == other._info && (_hasValue != other._hasValue || (_hasValue && _value != other._value)))
		return false;
	if (CommandFlagInfo::getContradictoriesList()->contains(std::pair(_info, other._info)))
		return false;
	return true;
}

bool CommandFlag::operator==(const CommandFlag& rhs) const
{
	return _info == rhs._info &&
		_hasValue == rhs._hasValue &&
		(!_hasValue || _value == rhs._value);
}

/// <summary>
/// Attempts to read the given input as a flag, returns null if it fails.
/// </summary>
/// <param name="input">The user input, not including any preceding flag markers such as '-'</param>
/// <returns>A pointer to a flag if the input was a valid flag, null otherwise.</returns>
CommandFlag* CommandFlag::read(std::wstring input)
{
	auto list = CommandFlagInfo::getList();
	size_t valueSeparatorPos = input.find(L'=');
	std::wstring flagCode;
	bool hasValue = valueSeparatorPos == input.npos;

	// Retrieve the flag code from input
	if (hasValue)
		flagCode = input.substr(0, valueSeparatorPos);
	else
		flagCode = input;

	// Retrieve the flag info, check it exists
	auto iter = list->search(true, &CommandFlagInfo::hasCode, flagCode);
	if (iter == list->npos())
		return nullptr;

	// Construct the flag, then return!
	CommandFlag* flag;
	if (!hasValue)
		flag = new CommandFlag(*iter);
	else
		flag = new CommandFlag(*iter, input.substr(valueSeparatorPos));
	return flag;
}

CommandFlagCollection::CommandFlagCollection()
{
	_data = easy_list::list<CommandFlag>();
}

CommandFlagCollection::CommandFlagCollection(const CommandFlag& flag)
{
	_data = easy_list::list<CommandFlag>({ flag });
}

bool CommandFlagCollection::isValid(CommandType commandType) const
{
	if (_data.contains(false, &CommandFlag::isValid, commandType))
		return false;
	if (_data.contains([this](CommandFlag flag1) -> bool { return _data.contains([flag1](CommandFlag flag2) -> bool { return flag1.contradicts(flag2); }); }))
		return false;
	return true;
}

CommandFlagCollection& CommandFlagCollection::add(const CommandFlag& rhs)
{
	return this->add((CommandFlagCollection)rhs);
}

CommandFlagCollection& CommandFlagCollection::add(const CommandFlagCollection& rhs)
{
	_data = _data.unify(rhs._data);
	return *this;
}

bool CommandFlagCollection::sharesAny(const CommandFlagCollection& other) const
{
	return _data.shares(other._data);
}