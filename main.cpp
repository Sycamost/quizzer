#include <iostream>
#include <vector>
#include "CmdHandler.h"
#include "CmdWrite.h"
#include "util.h"
#include "globals.h"
#include "Flashcard.h"
#include "Play.h"

CmdHandler::Returns defaultCmdHandler(std::wstring userInput);
CmdHandler::Returns(*cmdHandler)(std::wstring userInput) = &defaultCmdHandler;

int main() {

	std::wcout << L"Reading questions from file...\n";
	Question::readQuestionList();
	std::wcout << L"Read " << Question::getQuestionList().size() << L" questions from file.\n\n";
	std::wcout << L"Welcome to Quizzer!\n\n";

	while (true)
	{
		std::wstring userInput = getInputLine();
		CmdHandler::Returns ret = (*cmdHandler)(userInput);

		if (ret == CmdHandler::Returns::SUCCESS)
			continue;

		if (ret == CmdHandler::Returns::CMD_NOT_RECOGNISED)
		{
			std::wcout << L"Command not recognised. Please enter a valid command.\n";
			continue;
		}

		if (ret == CmdHandler::Returns::QUIT_APP)
		{
			break;
		}

		if (ret == CmdHandler::Returns::TOO_FEW_ARGS)
		{
			std::wcout << L"Too few arguments were provided for that command.\n";
			continue;
		}

		if (ret == CmdHandler::Returns::INVALID_ARGS)
		{
			std::wcout << L"Invalid arguments were provided for that command.\n";
			continue;
		}
	}

	return 0;
}

void CmdHandler::setHandler(Returns(*handler)(std::wstring))
{
	cmdHandler = handler;
}

void CmdHandler::setHandlerDefault()
{
	cmdHandler = &defaultCmdHandler;
}

CmdHandler::Returns defaultCmdHandler(std::wstring userInput)
{
	if (userInput.empty())
		return CmdHandler::Returns::SUCCESS;

	userInput = toUpper(userInput);
	std::vector<std::wstring> userInputWords = splitByWord(userInput);

	if (userInputWords[0] == Globals::cmdQuit || userInputWords[0] == Globals::cmdExit) {

		std::wcout << L"Are you sure you want to exit the app? [Y/N]\n";

		if (getUserYesNo())
		{
			std::wcout << L"Exiting app...\n";
			return CmdHandler::Returns::QUIT_APP;
		}

		std::wcout << L"Not exiting app.\n";
		return CmdHandler::Returns::SUCCESS;;
	}

	if (userInputWords[0] == Globals::cmdWrite)
	{
		if (userInputWords.size() <= 1)
			return CmdHandler::Returns::TOO_FEW_ARGS;

		if (userInputWords[1] == Globals::cmdWriteOptFlashcard)
		{
			startWritingFlashcards();
			return CmdHandler::Returns::SUCCESS;
		}

		return CmdHandler::Returns::INVALID_ARGS;
	}

	if (userInputWords[0] == Globals::cmdPlay)
	{
		startPlaying(slice(userInputWords, 1));
		return CmdHandler::Returns::SUCCESS;
	}

	return CmdHandler::Returns::CMD_NOT_RECOGNISED;
}
