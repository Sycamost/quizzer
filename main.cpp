#include <iostream>
#include "CmdHandler.h"
#include "Write.h"
#include "util.h"
#include "globals.h"

CmdHandler::Returns defaultCmdHandler(std::wstring userInput);
CmdHandler::Returns(*cmdHandler)(std::wstring userInput) = &defaultCmdHandler;

int main() {

	std::wcout << L"Welcome to flashcards!\n";

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
	userInput = toUpper(userInput);

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

	return CmdHandler::Returns::CMD_NOT_RECOGNISED;
}
