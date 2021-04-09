#include <algorithm>
#include "Command.h"
#include "CmdWrite.h"
#include "main.h"
#include "Play.h"
#include "util.h"

Command* Command::read(std::wifstream& stream)
{
	std::streampos pos = stream.tellg();
	std::wstring line = getInputLine(stream);
	if (line.size() == 0 || line[0] != L'\\')
	{
		stream.seekg(pos);
		return NULL;
	}
	return read(line);
}

Command* Command::read(std::wstring userInput)
{
	std::vector<std::wstring> words = splitByWord(userInput);
	if (words.size() == 0)
		return NULL;
	std::wstring cmd = words[0].substr(1);

	auto cmdInfoIter = std::find(_commandInfos.begin(), _commandInfos.end(), cmd);
	if (cmdInfoIter == Command::_commandInfos.end())
		return NULL;

	return new Command(
		std::vector<std::wstring>(words.begin() + 1, words.end(), std::allocator<std::wstring>()),
		*cmdInfoIter);
}

const CommandInfo* Command::getCommandInfo(CommandType type)
{
	auto ciIter = std::find_if(_commandInfos.begin(), _commandInfos.end(), CommandInfo::isType);
	if (ciIter == _commandInfos.end())
		return nullptr;
	return &(*ciIter);
}

const CommandInfo* Command::getCommandInfo(std::wstring code)
{
	auto ciIter = std::find_if(_commandInfos.begin(), _commandInfos.end(), CommandInfo::isCode);
	if (ciIter == _commandInfos.end())
		return nullptr;
	return &(*ciIter);
}

CmdHandler::Returns Command::doCommandFunc()
{
	return _commandInfo.func(_args);
}

CommandInfo Command::getCommandInfo()
{
	return _commandInfo;
}

Command::Command(std::vector<std::wstring> args, CommandInfo commandInfo)
{
	_args = args;
	_commandInfo = commandInfo;
}

const std::vector<CommandInfo> Command::_commandInfos = std::vector<CommandInfo>({
	CommandInfo(CommandType::CANCEL, L"CANCEL", cancelCurrentWrite),
	CommandInfo(CommandType::QUIT, L"QUIT", quitApp),
	CommandInfo(CommandType::EXIT, L"EXIT", quitApp),
	CommandInfo(CommandType::WRITE, L"WRITE", startWriting),
	CommandInfo(CommandType::CASE, L"CASE", setCaseSensitive),
	CommandInfo(CommandType::BOOST, L"BOOST", boost),
	CommandInfo(CommandType::FINISH, L"FINISH", finish),
	CommandInfo(CommandType::PLAY, L"PLAY", startPlaying)
});