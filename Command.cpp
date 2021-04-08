#include <utility>
#include "Command.h"
#include "Write.h"
#include "main.h"
#include "Play.h"
#include "util.h"

Command* Command::read(std::wifstream& stream)
{
	std::streampos pos = stream.tellg();
	std::wstring line = getUserInputLine(stream);
	if (line.size() == 0 || line[0] != L'\\')
	{
		stream.seekg(pos);
		return NULL;
	}

	std::vector<std::wstring> words = splitByWord(line);
	std::wstring cmd = words[0].substr(1);

	auto cmdFuncIter = Command::_cmdFuncs.find(cmd);
	if (cmdFuncIter == Command::_cmdFuncs.end())
		return NULL;

	return new Command(
		cmd,
		std::vector<std::wstring>(words.begin() + 1, words.end(), std::allocator<std::wstring>()),
		cmdFuncIter->second);
}

CmdHandler::Returns Command::doCmdFunc()
{
	return _cmdFunc(_args);
}

Command::Command(std::wstring cmd, std::vector<std::wstring> args, CmdFunc cmdFunc)
{
	_cmd = cmd;
	_args = args;
	_cmdFunc = cmdFunc;
}

typedef std::map<std::wstring, Command::CmdFunc> mapWstrCmdfunc;
typedef std::pair<std::wstring, Command::CmdFunc> pairWstrCmdfunc;
const mapWstrCmdfunc Command::_cmdFuncs = mapWstrCmdfunc({
	pairWstrCmdfunc(L"CANCEL", cancelCurrentWrite),
	pairWstrCmdfunc(L"QUIT", quitApp),
	pairWstrCmdfunc(L"EXIT", quitApp),
	pairWstrCmdfunc(L"WRITE", startWriting),
	pairWstrCmdfunc(L"CASE", setCaseSensitive),
	pairWstrCmdfunc(L"BOOST", boost),
	pairWstrCmdfunc(L"FINISH", finish),
	pairWstrCmdfunc(L"PLAY", startPlaying)});
const std::wstring Command::cmdWriteOptFlashcard = L"FLASHCARD";