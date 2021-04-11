#include <algorithm>
#include "Command.h"
#include "Write.h"
#include "main.h"
#include "Play.h"
#include "util.h"

const std::vector<CommandInfo> Command::_commandInfos = std::vector<CommandInfo>({
	CommandInfo(CommandType::QUIT, L"QUIT", &cmdFuncQuit),
	CommandInfo(CommandType::EXIT, L"EXIT", &cmdFuncQuit),
	CommandInfo(CommandType::WRITE, L"WRITE", &Write::cmdFuncWrite),
	CommandInfo(CommandType::CANCEL, L"CANCEL", &Write::cmdFuncCancel),
	CommandInfo(CommandType::BOOST, L"BOOST", &Play::cmdFuncBoost),
	CommandInfo(CommandType::FINISH, L"FINISH", &Play::cmdFuncFinish),
	CommandInfo(CommandType::PLAY, L"PLAY", &Play::cmdFuncPlay)
	});

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
	std::vector<std::wstring> words = splitByWord(toUpper(userInput));
	if (words.size() == 0)
		return nullptr;
	std::wstring code = words[0].substr(1);

	auto cmdInfoIter = std::find_if(
		_commandInfos.begin(),
		_commandInfos.end(),
		[code](CommandInfo ci) -> bool { return ci.isCode(code); });
	if (cmdInfoIter == Command::_commandInfos.end())
		return nullptr;

	return new Command(
		std::vector<std::wstring>(words.begin() + 1, words.end(), std::allocator<std::wstring>()),
		*cmdInfoIter);
}

const CommandInfo* Command::getCommandInfo(CommandType type)
{
	auto ciIter = std::find_if(
		_commandInfos.begin(),
		_commandInfos.end(),
		[type](CommandInfo ci) -> bool { return ci.isType(type); });
	if (ciIter == _commandInfos.end())
		return nullptr;
	return ciIter._Ptr;
}

const CommandInfo* Command::getCommandInfo(std::wstring code)
{
	auto ciIter = std::find_if(
		_commandInfos.begin(),
		_commandInfos.end(),
		[code](CommandInfo ci) -> bool { return ci.isCode(code); });
	if (ciIter == _commandInfos.end())
		return nullptr;
	return ciIter._Ptr;
}

CmdHandler::Returns Command::doCommandFunc()
{
	return (*_commandInfo.func)(_args);
}

CommandInfo Command::getCommandInfo()
{
	return _commandInfo;
}

const std::vector<std::wstring> Command::getArgs() const
{
	return _args;
}

Command::Command(std::vector<std::wstring> args, CommandInfo commandInfo) :
	_args(args),
	_commandInfo(commandInfo)
{}