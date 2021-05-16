#include <algorithm>
#include <easy_list.h>
#include <cstdarg>
#include "Command.h"
#include "Write.h"
#include "main.h"
#include "Play.h"
#include "util.h"
#include "HandlerReturns.h"

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
	easy_list::list<CommandInfo> list = *getList();
	auto iter = list.search(ct, &CommandInfo::getType);
	if (iter == list.npos())
		return L"#ERROR#";
	return iter->getFirstCode();
}

CommandFlags readCommandFlag(std::wstring input)
{
	auto list = CommandFlagInfo::getList();
	auto iter = list->search(true, &CommandFlagInfo::hasCode, input);
	if (iter == list->npos())
		return CommandFlags::NONE;
	return iter->getFlag();
}

/// <summary>
/// Finds all the commands which the given input could be a match for. (Doesn't check which commands are currently valid to actually call.)
/// </summary>
/// <param name="input">The input to interpret.</param>
/// <returns>A list of all commands that could ever be made from the given input.</returns>
easy_list::list<Command> Command::makePossibleCommands(std::wstring input)
{
	easy_list::list<std::wstring> words = splitByWord(toLower(input));
	if (words.size() == 0 || words[0].size() == 0 || words[0][0] != L'\\')
		return {};
	std::wstring code = words[0].substr(1);

	auto possibleCommandInfos = CommandInfo::getList()->select(true, &CommandInfo::hasCode, code);

	auto args = easy_list::list<std::wstring>();
	auto flags = CommandFlags::NONE;

	for each (auto word in words.slice(1))
	{
		if (!word.empty() && word[0] == L'-')
		{
			CommandFlags flag = readCommandFlag(word.substr(1));
			flags &= flag;
		}
		else
			args.push_back(word);
	}

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

const CommandFlags Command::getFlags() const
{
	return _flags;
}

Command::Command(easy_list::list<std::wstring> args, CommandFlags flags, CommandInfo commandInfo) :
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

const std::wstring CommandFlagInfo::getFirstCode(const CommandFlags flag)
{
	auto list = *getList();
	auto iter = list.search(flag, &CommandFlagInfo::getFlag);
	if (iter == list.npos())
		return L"";
	else
		return iter->getFirstCode();
}
