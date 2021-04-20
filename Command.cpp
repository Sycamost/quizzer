#include <algorithm>
#include <easy_list.h>
#include "Command.h"
#include "Write.h"
#include "main.h"
#include "Play.h"
#include "util.h"

const easy_list::list<CommandInfo>* CommandInfo::getList()
{
	static const auto list = easy_list::list<CommandInfo>({
		CommandInfo(CommandType::QUIT, L"QUIT", &cmdFuncQuit),
		CommandInfo(CommandType::EXIT, L"EXIT", &cmdFuncQuit),
		CommandInfo(CommandType::WRITE, L"WRITE", &Write::cmdFuncWrite),
		CommandInfo(CommandType::CANCEL, L"CANCEL", &Write::cmdFuncCancel),
		CommandInfo(CommandType::BOOST, L"BOOST", &Play::cmdFuncBoost),
		CommandInfo(CommandType::CONCEDE, L"CONCEDE", &Play::cmdFuncConcede),
		CommandInfo(CommandType::PLAY, L"PLAY", &Play::cmdFuncPlay)
	});
	return &list;
}

const easy_list::list<CommandInfo>::const_iterator CommandInfo::get(const CommandType type)
{
	return getList()->search(type, &CommandInfo::getType);
}

const easy_list::list<CommandInfo>::const_iterator CommandInfo::get(const std::wstring code)
{
	return getList()->search(code, &CommandInfo::getCode);
}

Command* Command::read(std::wifstream& stream)
{
	std::streampos pos = stream.tellg();
	std::wstring line = getInputLine(stream);
	if (line.size() == 0 || line[0] != L'\\')
	{
		stream.seekg(pos);
		return nullptr;
	}
	return read(line);
}

Command* Command::read(std::wstring userInput)
{
	easy_list::list<std::wstring> words = splitByWord(toUpper(userInput));
	if (words.size() == 0 || words[0].size() == 0 || words[0][0] != L'\\')
		return nullptr;
	std::wstring code = words[0].substr(1);

	auto cmdInfoIter = CommandInfo::get(code);
	if (cmdInfoIter == CommandInfo::getList()->npos())
		return nullptr;

	return new Command(words.slice(1), *cmdInfoIter);
}

InputHandler::Returns Command::doCommandFunc()
{
	return _commandInfo.callFunc(_args);
}

CommandInfo Command::getCommandInfo()
{
	return _commandInfo;
}

const easy_list::list<std::wstring> Command::getArgs() const
{
	return _args;
}

Command::Command(easy_list::list<std::wstring> args, CommandInfo commandInfo) :
	_args(args),
	_commandInfo(commandInfo)
{}