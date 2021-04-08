#include <utility>
#include "Command.h"
#include "Write.h"
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

	auto cmdInfoIter = Command::_commandInfos.find(cmd);
	if (cmdInfoIter == Command::_commandInfos.end())
		return NULL;

	return new Command(
		cmd,
		std::vector<std::wstring>(words.begin() + 1, words.end(), std::allocator<std::wstring>()),
		cmdInfoIter->second);
}

CmdHandler::Returns Command::doCommandFunc()
{
	return _commandInfo.func(_args);
}

CommandInfo Command::getCommandInfo()
{
	return _commandInfo;
}

Command::Command(std::wstring code, std::vector<std::wstring> args, CommandInfo commandInfo)
{
	_code = code;
	_args = args;
	_commandInfo = commandInfo;
}

typedef std::map<const std::wstring, const CommandInfo> mapCodeCmdInfo;
inline const std::pair<const std::wstring, const CommandInfo> pairCodeCmdInfo(const std::wstring code, const CommandType type, const CommandFunc func) {
	return std::pair<const std::wstring, const CommandInfo>(code, CommandInfo(type, func));
}
const mapCodeCmdInfo Command::_commandInfos = mapCodeCmdInfo({
	pairCodeCmdInfo(L"CANCEL", CommandType::CANCEL, cancelCurrentWrite),
	pairCodeCmdInfo(L"QUIT", CommandType::QUIT, quitApp),
	pairCodeCmdInfo(L"EXIT", CommandType::EXIT, quitApp),
	pairCodeCmdInfo(L"WRITE", CommandType::WRITE, startWriting),
	pairCodeCmdInfo(L"CASE", CommandType::CASE, setCaseSensitive),
	pairCodeCmdInfo(L"BOOST", CommandType::BOOST, boost),
	pairCodeCmdInfo(L"FINISH", CommandType::FINISH, finish),
	pairCodeCmdInfo(L"PLAY", CommandType::PLAY, startPlaying)});
const std::wstring Command::cmdWriteOptFlashcard = L"FLASHCARD";