#include <iostream>
#include "CmdHandler.h"

CmdHandler::Returns defaultCmdHandler(std::wstring userInput);
CmdHandler::Returns(*cmdHandler)(std::wstring userInput) = &defaultCmdHandler;

int main() {

	while (true)
	{
		std::wstring userInput = L"";
		std::wcin >> userInput;
		CmdHandler::Returns ret = (*cmdHandler)(userInput);

		if (ret == CmdHandler::Returns::SUCCESS)
			continue;

		if (ret == CmdHandler::Returns::CMD_NOT_RECOGNISED)
		{
			std::wcout << "Command not recognised. Please enter a valid command.\n";
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
	if (userInput == L"quit" || userInput == L"exit") {

		std::wcout << "Are you sure you want to quit? [Y/N]\n";
		std::wstring userInput = L"";
		std::wcin >> userInput;

		if (userInput == L"Y" || userInput == L"y"
			|| userInput == L"YES" || userInput == L"YEs" || userInput == L"YeS" || userInput == L"Yes" || userInput == L"yES" || userInput == L"yEs" || userInput == L"yeS" || userInput == L"yes"
			|| userInput == L"0" || userInput == L"true")
		{
			std::wcout << L"Exiting app...\n";
			return CmdHandler::Returns::QUIT_APP;
		}

		std::wcout << L"Not exiting app.\n";
		return CmdHandler::Returns::SUCCESS;;
	}

	return CmdHandler::Returns::CMD_NOT_RECOGNISED;
}
