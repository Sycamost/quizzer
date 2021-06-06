#include <algorithm>
#include <easy_list.h>
#include <cstdarg>
#include "Command.h"
#include "Write.h"
#include "main.h"
#include "Play.h"
#include "util.h"
#include "HandlerReturns.h"

bool CommandFlagInfo::operator==(const CommandFlagInfo& other) const
{
	return (const CommandFlagInfo*)this == &other;
}

bool CommandFlag::isValid() const
{
	return !_hasValue || !_value.empty();
}

bool CommandFlag::contradicts(CommandFlag other) const
{
	if (_info == other._info && (_hasValue != other._hasValue || (_hasValue && _value != other._value)))
		return false;
	return true;
}

bool CommandFlag::operator==(const CommandFlag& rhs) const
{
	return _info == rhs._info &&
		_hasValue == rhs._hasValue &&
		(!_hasValue || _value == rhs._value);
}

CommandFlagCollection::CommandFlagCollection()
{
	_data = easy_list::list<CommandFlag>();
}

CommandFlagCollection::CommandFlagCollection(const CommandFlag& flag)
{
	_data = easy_list::list<CommandFlag>({ flag });
}

bool CommandFlagCollection::isValid() const
{
	if (_data.contains(false, &CommandFlag::isValid))
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

bool CommandInfo::operator==(const CommandInfo& other)
{
	return _type == other._type;
}

const easy_list::list<CommandInfo>* CommandInfo::getList()
{
	static const easy_list::list<std::wstring> quitCodes{ L"quit", L"exit" };
	static const auto list = easy_list::list<CommandInfo>({
		CommandInfo(CommandType::QUIT_MAIN, quitCodes, &cmdFuncQuit),
		CommandInfo(CommandType::WRITE, { L"write" }, &Write::cmdFuncWrite),
		CommandInfo(CommandType::CANCEL, { L"cancel" }, &Write::cmdFuncCancel),
		CommandInfo(CommandType::QUIT_WRITE, quitCodes, &Write::cmdFuncQuitWrite),
		CommandInfo(CommandType::PLAY, { L"play" }, &Play::cmdFuncPlay),
		CommandInfo(CommandType::BOOST, { L"boost" }, &Play::cmdFuncBoost),
		CommandInfo(CommandType::CONCEDE, { L"concede" }, &Play::cmdFuncConcede),
		CommandInfo(CommandType::QUIT_PLAY, quitCodes, &Play::cmdFuncQuitPlay)
	});
	return &list;
}

const std::wstring CommandInfo::getFirstCode(const CommandType ct)
{
	auto list = getList();
	auto iter = list->search(ct, &CommandInfo::getType);
	if (iter == list->npos())
		return L"!ERROR!";
	return iter->getFirstCode();
}

/// <summary>
/// Attempts to read the given input as a flag, returns null if it fails.
/// </summary>
/// <param name="input">The user input, not including any preceding flag markers such as '-'</param>
/// <returns>A pointer to a flag if the input was a valid flag, null otherwise.</returns>
CommandFlag* readCommandFlag(std::wstring input)
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

	// Construct the flag, check it's valid, then return!
	CommandFlag* flag;
	if (!hasValue)
		flag = new CommandFlag(*iter);
	else
		flag = new CommandFlag(*iter, input.substr(valueSeparatorPos));
	if (!flag->isValid())
		return nullptr;
	return flag;
}

/// <summary>
/// Finds all the commands which the given input could be a match for. (Doesn't check which commands are currently valid to actually call.)
/// </summary>
/// <param name="input">The input to interpret.</param>
/// <returns>A list of all commands that could ever be made from the given input.</returns>
easy_list::list<Command> Command::makePossibleCommands(std::wstring input)
{
	easy_list::list<std::wstring> words = splitByWordOrQuotes(toLower(input));
	if (words.size() == 0 || words[0].size() == 0 || words[0][0] != L'\\')
		return {};
	std::wstring code = words[0].substr(1);

	auto possibleCommandInfos = CommandInfo::getList()->select(true, &CommandInfo::hasCode, code);

	auto args = easy_list::list<std::wstring>();
	auto flags = CommandFlagCollection();

	// Get args - keep going until we run out of words, or we start getting flags
	int i;
	for (i = 1; i < words.size(); i++)
	{
		std::wstring word = words[i];

		if (word.empty())
			continue;

		// If it's a flag, move on to processing flags!
		if (word[0] == L'-')
			break;

		args.push_back(word);
	}

	// Flags
	for (; i < words.size(); i++)
	{
		auto word = words[i];
		if (word[0] != L'-')
			break;
		auto flag = readCommandFlag(word.substr(1));
		if (flag != nullptr)
			flags.add(*flag);
	}
	if (!flags.isValid())
		return easy_list::list<Command>();

	return possibleCommandInfos.transform<Command>(
		[args, flags](CommandInfo ci) -> Command { return Command(args, flags, ci); }
	);
}

CommandHandlerReturns Command::doCommandFunc() const
{
	return _commandInfo.callFunc(_args, _flags);
}

const CommandInfo Command::getCommandInfo() const
{
	return _commandInfo;
}

const easy_list::list<std::wstring> Command::getArgs() const
{
	return _args;
}

const CommandFlagCollection Command::getFlags() const
{
	return _flags;
}

Command::Command(easy_list::list<std::wstring> args, CommandFlagCollection flags, CommandInfo commandInfo) :
	_args(args),
	_flags(flags),
	_commandInfo(commandInfo)
{}

const easy_list::list<CommandFlagInfo>* CommandFlagInfo::getList()
{
	static auto list = easy_list::list<CommandFlagInfo>({
		// CommandFlagInfo declarations go here
	});
	return &list;
}

const easy_list::list<easy_list::list<CommandFlagInfo>>* CommandFlagInfo::getContradictoriesList()
{
	static auto list = easy_list::list<easy_list::list<CommandFlagInfo>>({
		// Lists of contradictory CommandFlagInfos go here
	});
	return &list;
}
