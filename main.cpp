#include <iostream>
#include <vector>
#include "CmdHandler.h"
#include "Write.h"
#include "util.h"
#include "globals.h"
#include "Flashcard.h"
#include "Play.h"

CmdHandler::Returns defaultCmdHandler(std::wstring userInput);
CmdHandler::Returns(*cmdHandler)(std::wstring userInput) = &defaultCmdHandler;

int main() {

	std::wcout << L"Welcome to flashcards!\n";
	std::wcout << L"Reading flashcards from file...\n";
	Flashcard::readFlashcardList();
	std::wcout << L"Read " << Flashcard::getFlashcardList().size() << L" flashcards from file.\n\n";
	std::wcout << L"Welcome to flashcards!\n\n";

	while (true)
	{
		std::wstring userInput = getUserInputLine();
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

	if (userInput == Globals::cmdQuit || userInput == Globals::cmdExit) {

		std::wcout << L"Are you sure you want to exit the app? [Y/N]\n";

		if (getUserYesNo())
		{
			std::wcout << L"Exiting app...\n";
			return CmdHandler::Returns::QUIT_APP;
		}

		std::wcout << L"Not exiting app.\n";
		return CmdHandler::Returns::SUCCESS;;
	}

	if (userInput == Globals::cmdWrite)
	{
		startWriting();
		return CmdHandler::Returns::SUCCESS;
	}

	if (userInputWords[0] == Globals::cmdPlay)
	{
		play(slice(userInputWords, 1));
		return CmdHandler::Returns::SUCCESS;
	}

	return CmdHandler::Returns::CMD_NOT_RECOGNISED;
}
