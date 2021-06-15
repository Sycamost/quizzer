#include <algorithm>
#include <easy_list.h>
#include <cstdarg>
#include "Command.h"
#include "Write.h"
#include "main.h"
#include "Play.h"
#include "util.h"
#include "HandlerReturns.h"
#include "CommandFlag.h"

bool CommandInfo::operator==(const CommandInfo& other)
{
	return _type == other._type;
}

const easy_list::list<CommandInfo>* CommandInfo::getList()
{
	static const easy_list::list<std::wstring> quitCodes{ L"quit", L"exit" };
	static const auto list = easy_list::list<CommandInfo>({
		CommandInfo(
			CommandType::HELP,
			{ L"help", L"h" },
			L"lists all commands",
			&CommandInfo::cmdFuncHelp),
		CommandInfo(
			CommandType::QUIT_MAIN,
			quitCodes,
			L"quits the program (from the main loop)",
			&cmdFuncQuit),
		CommandInfo(
			CommandType::WRITE,
			{ L"write" },
			L"starts writing new questions",
			&Write::cmdFuncWrite),
		CommandInfo(
			CommandType::CANCEL,
			{ L"cancel" },
			L"when writing questions, cancels writing the current question",
			&Write::cmdFuncCancel),
		CommandInfo(
			CommandType::QUIT_WRITE,
			quitCodes,
			L"quits the program (from the main loop)",
			&Write::cmdFuncQuitWrite),
		CommandInfo(
			CommandType::PLAY,
			{ L"play" },
			L"starts play",
			&Play::cmdFuncPlay),
		CommandInfo(
			CommandType::BOOST,
			{ L"boost" },
			L"when playing, if the last question was marked incorrect, re-marks it as correct",
			&Play::cmdFuncBoost),
		CommandInfo(
			CommandType::CONCEDE,
			{ L"concede" },
			L"when playing, quits play early, skipping all remaining questions",
			&Play::cmdFuncConcede),
		CommandInfo(
			CommandType::QUIT_PLAY,
			quitCodes,
			L"quits the program (from play)",
			&Play::cmdFuncQuitPlay)
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

DEFINE_CMD_FUNC(CommandInfo::cmdFuncHelp)
{
	// List all commands
	std::wcout << L"\n";
	for (CommandInfo cmdInfo : *getList())
		std::wcout << cmdInfo.getFirstCode() << L" --- " << cmdInfo.getHelp() << L"\n";
	std::wcout << L"\n";
	return CommandHandlerReturns::SUCCESS;
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
		CommandFlag* flag = CommandFlag::read(word.substr(1));
		if (flag != nullptr)
			flags.add(*flag);
	}
	
	// Select only the command infos with the most valid flags
	int maxValidFlags = possibleCommandInfos.transform<int>([flags](CommandInfo ci) -> int { return ci.countValidFlags(flags); }).max();
	possibleCommandInfos = possibleCommandInfos.select(maxValidFlags, &CommandInfo::countValidFlags, flags);

	// For each possible command info, return a command containing all provided arguments and all valid flags
	return possibleCommandInfos.transform<Command>(
		[args, flags](CommandInfo ci) -> Command { return Command(args, flags.getList().select(true, &CommandFlag::isValid, ci.getType()), ci); }
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

int CommandInfo::countValidFlags(CommandFlagCollection flags)
{
	return flags.getList().select(true, &CommandFlag::isValid, _type).size();
}
